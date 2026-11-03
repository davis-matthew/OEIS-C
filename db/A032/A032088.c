/*
 * A032088  Number of reversible strings with n beads of 5 colors. If more
 *          than 1 bead, not palindromic.
 * https://oeis.org/A032088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 5 a(n) = 5^(⌊n/2⌋) · (5^(⌊n/2⌋) – 1) / 2 for even n a(n)
 *            = 5 · 5^(⌊n/2⌋) · (5^(⌊n/2⌋) – 1) / 2 for odd n > 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 188 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)5;                /* a(1) = 5 */

    /* m = floor(n/2) */
    idx_t m = n >> 1;                   /* unsigned shift is fine */

    /* compute p = 5^m */
    seq_t p = ipow((seq_t)5, (seq_t)m);

    /* base = p * (p - 1) / 2 */
    seq_t base = p * (p - 1) / 2;

    /* adjust for odd n */
    if (n & 1)
        base *= (seq_t)5;

    return base;
}
