/*
 * A028730  Nonsquares mod 17.
 * https://oeis.org/A028730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 17 * floor((n‑1)/8) + {3,5,6,7,10,11,12,14}[(n‑1) mod 8]
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    static const seq_t residues[8] = { 3, 5, 6, 7, 10, 11, 12, 14 };
    seq_t blocks  = (seq_t)(n - 1) / 8;      /* number of complete 17‑blocks */
    idx_t  offset = (n - 1) % 8;            /* position within the block   */
    return (seq_t)17 * blocks + residues[(size_t)offset];
}
