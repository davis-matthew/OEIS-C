/*
 * A093058  Numbers k such that numerator of Bernoulli(2k) is divisible by the
 *          square of 59, the second irregular prime.
 * https://oeis.org/A093058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n % 5 == 0 : (n/5) * 3481 else : 457 + ((m-1)/2) *
 *            1711 + ( (m % 2 == 0) ? 841 : 0 ) where m = n - floor(n/5)
 * Range:     u8 none, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the special multiples of 3481 */
    if (n % 5 == 0) {
        return (seq_t)(n / 5) * 3481;
    }

    /* Remaining terms follow an alternating pattern of +841 and +870 */
    idx_t m = n - (n / 5);            /* count of terms not divisible by 5 up to n */
    /* Use seq_t arithmetic everywhere */
    seq_t res = 457 + ((m - 1) / 2) * 1711;
    if ((m % 2) == 0) {
        res += 841;
    }
    return res;
}
