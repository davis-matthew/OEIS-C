/*
 * A086021  a(n) = Sum_{i=1..n} C(i+2,3)^3.
 * https://oeis.org/A086021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+3,4) * (1 + 12*C(n-1,1) + 46*C(n-1,2) + 84*C(n-1,3) *
 *            + 81*C(n-1,4) + 40*C(n-1,5) + 8*C(n-1,6))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 180, u128 n <= 5000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Ensure the provided binomial function is accessible */
#include "../utils/binom.h"   /* Assume binom(idx_t a, idx_t b) is defined here */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 the function is defined as 0 */
    if (n == 0) return 0;

    idx_t n_minus_1 = n - 1;

    /* Sum of weighted binomials */
    seq_t sum = 1;                                 /* 1 = C(n-1,0) */
    sum += (seq_t)12 * binom(n_minus_1, 1);
    sum += (seq_t)46 * binom(n_minus_1, 2);
    sum += (seq_t)84 * binom(n_minus_1, 3);
    sum += (seq_t)81 * binom(n_minus_1, 4);
    sum += (seq_t)40 * binom(n_minus_1, 5);
    sum += (seq_t)8  * binom(n_minus_1, 6);

    /* Multiplying by C(n+3,4) */
    seq_t coeff = binom(n + 3, 4);

    return coeff * sum;
}
