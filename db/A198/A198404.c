/*
 * A198404  a(n) = 8^n*n^8.
 * https://oeis.org/A198404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n * n^8
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 11, u128 n <= 29,
 *            big any n
 * Helpers:   pow_seq, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pow8 = pow_uint(8, n);        /* 8^n */
    seq_t pown = pow_seq((seq_t)n, 8);  /* n^8 */
    return pow8 * pown;                /* 8^n * n^8 */
}
