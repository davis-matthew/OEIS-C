/*
 * A054688  Number of nonnegative integer n X n matrices with sum of elements
 *          equal to n; polynomial symmetric functions of matrix of order n.
 * https://oeis.org/A054688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n^2 + n - 1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial function is provided by the utils library */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly */
    if (n == 0)
        return (seq_t)1;

    /* Compute N = n^2 + n - 1 using 64‑bit arithmetic (safe for n <= 10000) */
    idx_t N = n * n + n - 1;

    /* Return the binomial coefficient C(N, n).  The utils implementation
     * handles arbitrary‑precision construction of the result in seq_t. */
    return binomial(N, n);
}
