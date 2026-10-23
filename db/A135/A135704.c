/*
 * A135704  a(n) = 7*n^2 + 4*n + 1.
 * https://oeis.org/A135704
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 + 4*n + 1 (offset -1: a(-1) = 4)
 * Range:     u8 n <= 5, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the n-th term of A135704 for n ≥ -1. */
seq_t a(idx_t n)
{
    /* Special case for the negative offset index. */
    if (n == -1) {
        return (seq_t)4;
    }

    /* Compute 7*n^2 + 4*n + 1 using unsigned arithmetic. */
    seq_t nn = (seq_t)n;           /* n is non‑negative here */
    return (seq_t)7 * nn * nn + (seq_t)4 * nn + (seq_t)1;
}
