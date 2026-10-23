/*
 * A047585  Numbers that are congruent to {0, 1, 3, 5, 6, 7} mod 8.
 * https://oeis.org/A047585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/6) * 8 + [0,1,3,5,6,7][(n-1)%6]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _allowed[6] = {0, 1, 3, 5, 6, 7};

seq_t a(idx_t n)
{
    /* n is 1‑based. Compute zero‑based index. */
    idx_t zero = n - 1;
    idx_t q = zero / 6;          /* how many complete blocks of 8 */
    idx_t r = zero % 6;          /* position inside current block */

    /* Compute the value. Use seq_t to avoid promotion issues. */
    seq_t val = (seq_t)q * (seq_t)8 + _allowed[r];
    return val;
}
