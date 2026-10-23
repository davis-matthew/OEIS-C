/*
 * A117865  Number of palindromes (in base 6) below 6^n.
 * https://oeis.org/A117865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*6^((n-1)/2)-2 (n odd), 2*6^(n/2)-2 (n even)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; protect against n=0 */
        return (seq_t)0;

    idx_t    exp;
    seq_t    pow, res;

    if ((n & 1) == 1) {    /* n is odd */
        exp = (n - 1) / 2;
        pow = big_pow((seq_t)6, (seq_t)exp);
        res = (seq_t)7 * pow;
    } else {               /* n is even */
        exp = n / 2;
        pow = big_pow((seq_t)6, (seq_t)exp);
        res = (seq_t)2 * pow;
    }

    return res - (seq_t)2;
}
