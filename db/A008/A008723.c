/*
 * A008723  Molien series for 3-dimensional group [2,11] = *2 2 11.
 * https://oeis.org/A008723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(15+11*(-1)^n) + 82*(-1)^n + 86)/88) - [(n
 *            mod 11)=9]
 * Range:     u8 n <= 137, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a larger intermediate type to avoid overflow */
    unsigned __int128 num = (unsigned __int128)n * n;
    if ((n & 1) == 0) {               /* n even  */
        num += (unsigned __int128)26 * n + 168;
    } else {                           /* n odd   */
        num += (unsigned __int128)4 * n + 4;
    }

    seq_t res = (seq_t)(num / 88);

    if ((n % 11) == 9) {
        res -= (seq_t)1;
    }
    return res;
}
