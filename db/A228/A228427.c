/*
 * A228427  Denominator of Sum_{i=1..n-1} 1/i^(2*n-1).
 * https://oeis.org/A228427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{i=1}^{n-1} 1 / i^(2*n-1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, floor_log_u64, mod_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_log_u64.h"

static uint64_t pow_uint64(uint64_t base, uint32_t exp)
{
    uint64_t r = 1;
    while (exp) {
        if (exp & 1) r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Generates primes up to n-1 */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    if (n > 1)
        build_primes_upto(n - 1, &primes, &prime_cnt);

    /* If no primes, answer is 1 (n = 1 or 2) */
    if (prime_cnt == 0) {
        seq_t one = (seq_t)1;
        free(primes);
        return one;
    }

    seq_t result = (seq_t)1;
    uint64_t mod2n1 = (uint64_t)(2 * n - 1);

    /* Precompute floor logs for each prime */
    uint64_t *max_log = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        max_log[i] = floor_log_u64(n - 1, primes[i]);

    /* For each prime p */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t mp = max_log[pi];             /* floor_log_p(n-1) */
        uint64_t E_p = mod2n1 * mp;            /* exponent in lcm */

        /* mPow = p^mp  (fits in uint64_t because mp <= 13 for n <= 10000) */
        uint64_t mPow = pow_uint64(p, (uint32_t)mp);

        uint64_t limit_t = (n - 1) / mPow;     /* maximum t  */
        /* Determine v_p(S) */
        uint64_t v_p = 0;                      /* exponent of p in the sum */

        /* Iterate over k: find first k such that sum mod p^k != 0 */
        for (uint32_t k = 1; ; ++k) {
            seq_t mod = (seq_t)pow_uint64(p, k);
            seq_t phi = mod - mod / (seq_t)p;   /* φ(p^k) = p^k - p^(k-1) */
            uint64_t phi_minus1 = (uint64_t)(phi - (seq_t)1);

            /* Compute Z_p(k) = ∏_{q≠p} q^{E_q}  (mod mod) */
            seq_t Z = (seq_t)1;
            for (size_t qi = 0; qi < prime_cnt; ++qi) {
                if (qi == pi) continue;
                uint64_t q = primes[qi];
                uint64_t exp = mod2n1 * max_log[qi];
                Z = (Z * (seq_t)mod_pow((seq_t)q, exp, mod)) % mod;
            }

            /* Sum over t */
            seq_t sum = (seq_t)0;
            for (uint64_t t = 1; t <= limit_t; ++t) {
                if (t % p == 0) continue;      /* t must be coprime to p */
                seq_t t_pow = (seq_t)mod_pow((seq_t)t, mod2n1, mod);
                seq_t inv = (seq_t)mod_pow(t_pow, phi_minus1, mod);
                seq_t term = (Z * inv) % mod;
                sum += term;
                if (sum >= mod) sum -= mod;
            }

            if (sum != 0) {
                v_p = k - 1;
                break;
            }
            /* Continue to next k */
        }

        /* Remaining exponent in denominator after cancelling gcd */
        seq_t pow_rem = big_pow((seq_t)p, (seq_t)(E_p - v_p));
        result = seq_mul(result, pow_rem);
    }

    /* Clean up */
    free(primes);
    free(max_log);

    return result;
}
