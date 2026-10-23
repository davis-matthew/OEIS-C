/*
 * A047590  Numbers that are congruent to {0, 6, 7} mod 8.
 * https://oeis.org/A047590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n-1)/3) + r, where r = 0 if (n-1)%3 == 0, r = 6 if
 *            (n-1)%3 == 1, and r = 7 if (n-1)%3 == 2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for n < 1, return 0 as a defensive fallback */
        return (seq_t)0;

    idx_t g   = (n - 1) / 3;      /* number of complete triples of terms   */
    idx_t rem = (n - 1) % 3;      /* position inside the current triple */

    seq_t r = 0;
    switch (rem) {
        case 1: r = 6; break;
        case 2: r = 7; break;
        default: /* rem == 0 */ r = 0; break;
    }

    /* Perform arithmetic in seq_t to avoid overflow on idx_t types. */
    return ((seq_t)8 * (seq_t)g) + r;
}
