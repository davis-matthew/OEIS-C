/*
 * A144545  a(n) = 2^(n*(n-1))*(2^n + 1)*Product_{i=1..n-1} (4^i - 1).
 * https://oeis.org/A144545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n-1))*(2^n + 1)*Product_{i=1}^{n-1} (4^i - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided pow2 utility to compute 2^k efficiently. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n*(n-1)) */
    idx_t exp = n * (n - 1);
    seq_t res = pow2(exp);

    /* Multiply by (2^n + 1) */
    seq_t term = pow2(n) + 1;
    res *= term;

    /* If n <= 1, the product is empty and we are done */
    if (n <= 1)
        return res;

    /* Product over i=1..n-1 of (4^i - 1) */
    /* Start with 4^1 = 4 (i.e., pow2(2)) */
    seq_t fourth = pow2(2);   /* 4^1 */
    for (idx_t i = 1; i < n; ++i) {
        res *= (fourth - 1); /* multiply by (4^i - 1) */
        fourth *= 4;         /* produce 4^(i+1) for next iteration */
    }

    return res;
}
