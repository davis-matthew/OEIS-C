/*
 * A078222  A(1) = 2, a(n+1) > a(n) is the smallest multiple of a(n) using
 *          only even digits.
 * https://oeis.org/A078222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1)=2, a(2)=4, a(3)=8, a(2n)=24*10^(n-2), a(2n+1)=48*10^(n-2)
 *            for n>1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* even indices: n = 2k, k > 1  -> a(2k)=24*10^(k-2) */
    if ((n & 1) == 0) {
        idx_t k   = n / 2;          /* k > 1 here */
        idx_t exp = k - 2;          /* exponent of 10 */
        return (seq_t)24 * pow10(exp);
    }

    /* odd indices: n = 2k+1, k > 1  -> a(2k+1)=48*10^(k-2) */
    {
        idx_t k   = (n - 1) / 2;    /* k > 1 here */
        idx_t exp = k - 2;          /* exponent of 10 */
        return (seq_t)48 * pow10(exp);
    }
}
