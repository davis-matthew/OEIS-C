/*
 * A047485  Numbers that are congruent to {0, 3, 5, 7} mod 8.
 * https://oeis.org/A047485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,3,5,7][(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t offsets[4] = {0, 3, 5, 7};

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is undefined, but we guard it anyway */
    if (n == 0) {
        return 0;
    }
    idx_t k = n - 1;          /* zero‑based index of the term */
    idx_t q = k / 4;          /* number of full blocks of 4 terms */
    idx_t r = k % 4;          /* position within the current block */

    return (seq_t)q * (seq_t)8 + offsets[r];
}
