/*
 * A131520  Number of partitions of the graph G_n (defined below) into
 *          "strokes".
 * https://oeis.org/A131520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n - 1) + 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1; behaviour for n < 1 is undefined. */
    if (n == 0)
        return 0; /* optional guard */

    seq_t pow2n = pow2(n);                 /* 2^n */
    seq_t two_n_minus_two = (seq_t)2 * ((seq_t)n - 1); /* 2*(n-1) */
    return pow2n + two_n_minus_two;
}
