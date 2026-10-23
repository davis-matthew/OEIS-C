/*
 * A048478  a(n) = T(6,n), array T given by A048472.
 * https://oeis.org/A048478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n * 2^(n-1) + 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A048478, Offset: 0 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn   = (seq_t)n;       /* cast index to seq_t */
    seq_t pw   = pow2(n - 1);    /* 2^(n-1) */
    seq_t prod = 7 * nn * pw;    /* 7*n*2^(n-1) */
    return prod + 1;
}
