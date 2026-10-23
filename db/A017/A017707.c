/*
 * A017707  Numerator of sum of -22nd powers of divisors of n.
 * https://oeis.org/A017707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 56,
 *            big any n
 * Helpers:   factor_u64, seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of sigma_22(n)/n^22: a(n) = sigma_22(n)/gcd(sigma_22(n), n^22) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Factorize n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t sigma = (seq_t)1;  /* product over prime powers */

    for (size_t idx = 0; idx < pcnt; ++idx) {
        uint64_t p  = primes[idx];
        uint64_t e  = exps[idx];

        /* Sum_{i=0..e} p^{22i} computed iteratively */
        seq_t term   = (seq_t)1;          /* start with i=0 */
        seq_t factor = (seq_t)1;          /* p^{22*i} for current i */
        seq_t p22 = seq_pow((seq_t)p, 22ULL);

        for (uint64_t i = 1; i <= e; ++i) {
            factor = seq_mul(factor, p22);
            term = term + factor;
        }

        sigma = seq_mul(sigma, term);
    }

    seq_t n_pow = seq_pow((seq_t)n, 22ULL);
    seq_t g = seq_gcd(sigma, n_pow);
    return sigma / g;
}
