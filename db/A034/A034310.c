/*
 * A034310  Decimal part of a(n)^(1/4) starts with reversal of its integer
 *          part: first term of runs.
 * https://oeis.org/A034310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For m = n+1, let rev(m) be the decimal string of m reversed
 *            (including leading zeros), and let L = number of digits of
 *            rev(m). a(n) is the smallest integer a > m^4 for which a^(1/4) ∈
 *            [m + rev(m)/10^L , m + (rev(m)+1)/10^L). It is computed as a =
 *            ceil( ( (m*10^L + rev(m))^4 ) / (10^L)^4 )
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n + 1;                      /* integer part of the fourth root */

    /* Compute reverse of m and its digit count */
    uint64_t rev = 0;
    unsigned int len = 0;
    uint64_t tmp = (uint64_t)m;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
        len++;
    }

    /* 10^len as 128-bit integer */
    unsigned __int128 pow10 = 1;
    for (unsigned int i = 0; i < len; ++i) pow10 *= 10;

    /* numerator = m*10^len + rev, denominator = 10^len */
    unsigned __int128 num = (unsigned __int128)m * pow10 + rev;
    unsigned __int128 den = pow10;

    /* Compute num^4 and den^4 */
    unsigned __int128 num4 = num * num * num * num;
    unsigned __int128 den4 = den * den * den * den;

    /* ceil division: (num4 + den4 - 1) / den4 */
    unsigned __int128 a128 = (num4 + den4 - 1) / den4;

    return (seq_t)a128;
}
