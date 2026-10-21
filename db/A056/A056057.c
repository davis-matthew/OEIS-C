/*
 * A056057  The largest square which divides n-th central binomial
 *          coefficient.
 * https://oeis.org/A056057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest square dividing binomial(n, floor(n/2))
 * Range:     u8 n <= 26, u16 n <= 125, u32 n <= 362, u64 n <= 1594,
 *            u128 n <= 6240, big any n
 * Helpers:   build_primes_upto, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A056057: a(n) = A008833(A001405(n))) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

/* Helper: exponent of prime p in m! */
static idx_t vp_factorial(idx_t m, uint64_t p)
{
    idx_t sum = 0;
    idx_t power = p;
    while (power <= m) {
        sum += m / power;
        if (power > m / p) break;      /* avoid overflow */
        power *= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n >= 1 */

    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static uint64_t max_prime_limit = 0;

    /* Ensure we have all primes <= n for the factorisation */
    if ((uint64_t)n > max_prime_limit) {
        if (primes) free(primes);        /* free old list if any */
        build_primes_upto(n, &primes, &prime_cnt);
        max_prime_limit = n;
    }

    const idx_t k = n / 2;               /* floor(n/2) */
    seq_t result = 1;                    /* product of p^{2*floor(e/2)} */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n) break;                /* no larger primes needed */

        idx_t exp = vp_factorial(n, p)
                  - vp_factorial(k,   p)
                  - vp_factorial(n - k, p);

        exp &= ~((idx_t)1);              /* keep the even part only */
        if (exp == 0) continue;

        seq_t p_pow = pow_seq((seq_t)p, (int)exp);
        seq_t tmp    = seq_mul(result, p_pow);
        result       = tmp;
    }

    return result;
}
