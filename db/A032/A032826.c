/*
 * A032826  Numbers whose set of base-14 digits is {1,4}.
 * https://oeis.org/A032826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) enumerates numbers whose base‑14 representation uses only
 *            digits 1 and 4, in increasing order
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 766, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the number of base‑14 digits L (L ≥ 1).  L is the
       floor of log2(n + 1).  For n = 1→L = 1, n = 2→L = 1, n = 3→L = 2, … */
    idx_t m = n + 1;
    idx_t L = 0;
    while (m > 1) {
        m >>= 1;
        ++L;
    }

    /* Count how many numbers have fewer than L digits: 2^L – 2.  */
    idx_t lower = ((idx_t)1 << L) - 2;

    /* Zero‑based position of n within the block of L‑digit numbers.  */
    idx_t pos = n - lower - 1;

    /* Build the decimal value from the digits.  Each bit of pos,
       from most significant to least significant, selects a
       base‑14 digit: 0 → 1, 1 → 4. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        idx_t shift = L - 1 - i;
        idx_t bit = (pos >> shift) & 1;
        val = val * 14 + (bit ? 4 : 1);
    }
    return val;
}
