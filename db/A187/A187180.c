/*
 * A187180  Parse the infinite string 0101010101... into distinct phrases 0,
 *          1, 01, 010, 10, ...; a(n) = length of n-th phrase.
 * https://oeis.org/A187180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n <= 2 a(n) = 2 + 2*floor((n-3)/4) + ((n-3) % 4 &
 *            1) for n > 2 This implements the OEIS definition for A187180
 * Range:     u8 n <= 510, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;

    idx_t m = n - 3;          /* non‑negative */
    idx_t b = m / 4;          /* block index */
    idx_t r = m % 4;          /* position inside block */

    /* Compute the term in the current block */
    seq_t val = (seq_t)(2 + 2 * b + (r & 1));
    return val;
}
