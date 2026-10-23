/*
 * A025581  Triangle read by rows: T(n, k) = n-k, for 0 <= k <= n.
 * https://oeis.org/A025581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = r*(r+3)/2 - n, where r = floor((sqrt(1+8*n)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 1753, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Returns the n-th term of OEIS A025581 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)1 + (seq_t)8 * nn;        /* 1 + 8*n */
    seq_t s   = floor_sqrt(val);                /* floor(sqrt(1+8*n)) */
    seq_t r   = (s - (seq_t)1) / (seq_t)2;      /* row index */
    seq_t tmp = r * (r + (seq_t)3);
    tmp = tmp / (seq_t)2;                       /* r*(r+3)/2 */
    seq_t result = tmp - nn;                    /* subtract n */
    return result;
}
