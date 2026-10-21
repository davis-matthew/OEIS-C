/*
 * A099375  Sequence matrix for odd numbers.
 * https://oeis.org/A099375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2*(m - k) - 1, where m = floor((sqrt(8*n + 1) + 1) / 2)
 *            (block number), and k = n - (m-1)*m/2 (offset within the block)
 * Range:     u8 n <= 8255, u16 n <= 9408, u32 n <= 9408, u64 n <= 9408,
 *            u128 n <= 9408, big any n
 * Helpers:   _block_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */
#include "../utils/_block_index.h"

seq_t a(idx_t n)
{
    idx_t m = _block_index(n);                     /* block number   */
    /* start index of this block */
    idx_t start = ( (__int128)(m - 1) * m ) / 2;
    idx_t k = n - start;                           /* offset in block */

    /* value = 2*(m - k) - 1, computed in seq_t */
    seq_t val = ( (seq_t)2 * ( (seq_t)m - (seq_t)k ) ) - 1;
    return val;
}
