/*
 * A073807  Number of divisors of sum of cube of divisors.
 * https://oeis.org/A073807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d( sum_{d|n} d^3 )
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 2150, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor n, compute the sum of cubes of its divisors,
   factor that sum, and multiply the (exponent+1) of its prime factors
   to obtain the divisor count.  This works for all n for which the
   intermediate values fit into 64‑bit integers (which includes all
   test values up to 10000). */
#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) { /* never used, but keep offset compliance */
        return (seq_t)0;
    }

    /* 1. Factor n */
    uint64_t primes_n[32];
    int exps_n[32];
    int pcnt_n = 0;
    factor_uint64((uint64_t)n, primes_n, exps_n, &pcnt_n);

    /* 2. Compute S = ∑_{d|n} d^3 using multiplicativity */
    seq_t sum_cubes = (seq_t)1;          /* product over prime factors */

    for (int i = 0; i < pcnt_n; ++i) {
        seq_t p  = (seq_t)primes_n[i];
        seq_t p3 = p * p * p;           /* p^3 */

        /* geometric sum 1 + p^3 + p^6 + ... + p^{3*exp} */
        seq_t term = (seq_t)1;          /* k = 0 term */
        seq_t pw   = (seq_t)1;          /* current power p^{3k} */

        for (int k = 1; k <= exps_n[i]; ++k) {
            pw *= p3;                  /* p^{3k} */
            term += pw;                /* add to sum */
        }
        sum_cubes *= term;
    }

    /* 3. Factor the sum (now cast to 64‑bit, safe for the test range) */
    uint64_t sum64 = (uint64_t)sum_cubes;  /* fits in 64 bits for n ≤ 10000 */
    uint64_t primes_s[32];
    int exps_s[32];
    int pcnt_s = 0;
    factor_uint64(sum64, primes_s, exps_s, &pcnt_s);

    /* 4. Count divisors: product of (exponent+1) */
    seq_t divcnt = (seq_t)1;
    for (int i = 0; i < pcnt_s; ++i) {
        divcnt *= (seq_t)(exps_s[i] + 1);
    }

    return divcnt;
}
