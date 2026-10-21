/*
 * A060107  Numbers that are congruent to {0, 2, 3, 5, 7, 8, 10} mod 12. The
 *          ivory keys on a piano, start with A0 = the 0th key.
 * https://oeis.org/A060107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12 * floor((n-1)/7) + {0,2,3,5,7,8,10}[(n-1) mod 7]
 * Range:     u8 n <= 150, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; safe-guard though usage starts at 1 */
        return 0;

    idx_t m = n - 1;          /* 0‑based index */
    idx_t q = m / 7;          /* number of full 7‑term groups */
    idx_t r = m % 7;          /* position within the group */

    static const seq_t offs[7] = {0, 2, 3, 5, 7, 8, 10};

    /* The multiplication is done in seq_t to avoid intermediate overflow,
       which is safe because q is small for all practical n where a(n) fits
       in seq_t.                                            */
    return (seq_t)12 * (seq_t)q + offs[(size_t)r];
}
