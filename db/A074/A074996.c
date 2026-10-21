/*
 * A074996  Floor of concatenation of n, n+1, n+2, n+3, n+4, n+5 divided by 6.
 * https://oeis.org/A074996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(concat(n, n+1, n+2, n+3, n+4, n+5)/6)
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; i < 6; ++i)
    {
        idx_t num = n + i;            /* current integer to concatenate */
        int len = digits(num);        /* number of decimal digits of num   */
        result = result * pow10((idx_t)len) + (seq_t)num;
    }
    return result / 6;                 /* floor division by 6 */
}
