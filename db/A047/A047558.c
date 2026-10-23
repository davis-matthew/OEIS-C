/*
 * A047558  Numbers that are congruent to {1, 3, 6, 7} mod 8.
 * https://oeis.org/A047558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)/4)*8 + {1,3,6,7}[(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; return 0 for out‑of‑range indices */
    if (n == 0) return 0;

    /* Number of complete blocks of four terms before n */
    idx_t block = (n - 1) / 4;
    /* Position inside the current block: 0 → 1, 1 → 3, 2 → 6, 3 → 7 */
    idx_t rem   = (n - 1) % 4;

    static const seq_t r[4] = { 1, 3, 6, 7 };
    seq_t base = (seq_t)block * 8;

    return base + r[(unsigned int)rem];
}
