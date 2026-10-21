/*
 * A206918  Sum of binary palindromes p < 2^n.
 * https://oeis.org/A206918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = * a(0) = 0; * for n > 0: * k = floor(n/2); * t = 2^(3*k);
 *            * if n is even: a(n) = (9*t - 16)/14; * if n is odd : a(n) =
 *            (15*t - 8)/7;
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t k = n / 2;                     /* floor(n/2) */
    seq_t t = pow2(3 * k);              /* t = 2^(3*k) */

    if ((n & 1) == 0) {                 /* even n */
        seq_t val = t * 9;              /* 9*t */
        val -= 16;                      /* 9*t - 16 */
        val /= 14;                      /* divide by 14 */
        return val;
    } else {                            /* odd n */
        seq_t val = t * 15;             /* 15*t */
        val -= 8;                       /* 15*t - 8 */
        val /= 7;                       /* divide by 7 */
        return val;
    }
}
