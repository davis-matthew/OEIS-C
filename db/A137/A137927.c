/*
 * A137927  a(n) = the largest divisor of A000005(n) that is coprime to n.
 *          (A000005(n) = the number of positive divisors of n.).
 * https://oeis.org/A137927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest divisor of τ(n) that is coprime to n
 * Range:     u8 n <= 3339, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here τ(n) is the divisor count A000005(n).                                 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    /* Factorize n and compute τ(n) */
    uint64_t primes_n[64];
    uint64_t exps_n[64];
    size_t cnt_n = 0;
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);

    seq_t tau = 1;
    for (size_t i = 0; i < cnt_n; ++i) {
        tau = tau * (seq_t)(exps_n[i] + 1);
    }

    /* τ(n) fits easily into a uint64_t for the intended range */
    uint64_t tau_u = (uint64_t)tau;
    if (tau_u == 0) { /* overflow, fallback to zero divisor */
        return (seq_t)0;
    }

    /* Factor τ(n) to generate its divisors */
    uint64_t primes_t[64];
    uint64_t exps_t[64];
    size_t cnt_t = 0;

    idx_t sqrt_limit = _floor_isqrt_u64(tau_u);
    uint64_t *primes_list = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(sqrt_limit, &primes_list, &primes_cnt);

    uint64_t remaining = tau_u;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_list[i];
        if (p * p > remaining)
            break;
        if (remaining % p == 0) {
            uint64_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            primes_t[cnt_t] = p;
            exps_t[cnt_t] = exp;
            ++cnt_t;
        }
    }
    if (remaining > 1) {
        primes_t[cnt_t] = remaining;
        exps_t[cnt_t] = 1;
        ++cnt_t;
    }
    free(primes_list);

    /* Generate all divisors of τ(n) */
    seq_t divisors[256];
    size_t div_count = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < cnt_t; ++i) {
        uint64_t p = primes_t[i];
        uint64_t e  = exps_t[i];
        size_t prev = div_count;
        for (size_t j = 0; j < prev; ++j) {
            seq_t base = divisors[j];
            seq_t mult = 1;
            for (uint64_t k = 0; k < e; ++k) {
                mult = mult * (seq_t)p;
                divisors[div_count++] = base * mult;
            }
        }
    }

    /* Find the largest divisor coprime to n */
    seq_t best = 0;
    seq_t n_seq = (seq_t)n;
    for (size_t i = 0; i < div_count; ++i) {
        seq_t d = divisors[i];
        if (gcd(d, n_seq) == (seq_t)1) {
            if (d > best)
                best = d;
        }
    }

    return best;
}
