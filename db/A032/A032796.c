/*
 * A032796  Numbers that are congruent to {1, 2, 3, 5, 6} mod 7.
 * https://oeis.org/A032796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/5) + {1,2,3,5,6}[(n-1) mod 5]
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based index, OEIS offset 1 */
    idx_t k = (n - 1) / 5;          /* block number (0‑based) */
    idx_t r = (n - 1) % 5;          /* position inside the block (0‑5) */
    static const seq_t offset[5] = {1, 2, 3, 5, 6};
    return (seq_t)(7 * k + offset[(size_t)r]);
}
