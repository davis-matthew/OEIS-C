/*
 * A193242  a(n) = (C(n, floor(n/2)) + 2)^n for n >= 0.
 * https://oeis.org/A193242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(n, floor(n/2)) + 2)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(n, floor(n/2)) */
    idx_t k = n / 2;                 /* floor(n/2) */
    seq_t binom = (seq_t)1;
    idx_t i;

    for (i = (idx_t)1; i <= k; ++i) {
        binom = binom * (seq_t)(n - k + i);   /* multiply by (n - k + i) */
        binom = binom / (seq_t)i;             /* divide by i – exact division */
    }

    /* Add 2 to the binomial coefficient */
    seq_t base = binom + (seq_t)2;

    /* Raise to the power n */
    seq_t result = big_pow(base, (seq_t)n);

    return result;
}
