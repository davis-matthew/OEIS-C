/*
 * A008490  Expansion of (1-x^8) / (1-x)^8.
 * https://oeis.org/A008490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (for n = 0) a(n) = (180 + 469*n^2 + 70*n^4 + n^6) / 90
 *            for n > 0 (Derived from the expansion of (1 - x^8) / (1 - x)^8)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* Use unsigned __int128 for intermediate calculations to avoid overflow
       for n up to at least 10 000. */
    unsigned __int128 nn  = n;
    unsigned __int128 n2  = nn * nn;       /* n^2   */
    unsigned __int128 n4  = n2 * n2;        /* n^4   */
    unsigned __int128 n6  = n4 * n2;        /* n^6   */
    unsigned __int128 num = 180
                          + 469 * n2
                          + 70  * n4
                          + n6;
    return (seq_t)(num / 90);
}
