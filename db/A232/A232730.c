/*
 * A232730  Number of n-digit numbers that yield an (n+1)-digit number after
 *          Reverse and Add.
 * https://oeis.org/A232730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 5, a(2) = 45, a(3) = 495. For odd n > 2: a(n) = 99 *
 *            2^(n-3) * 5^(n-2). For even n > 2: a(n) = 45 * (11*10^(n-3) -
 *            10^(n/2-2)). All arithmetic is on seq_t and uses the provided
 *            power routines
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow2.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)45;
    if (n == 3) return (seq_t)495;

    if (n & 1) {                     /* n is odd and > 3 */
        seq_t res = (seq_t)99;
        res *= pow2(n - 3);
        res *= power5(n - 2);
        return res;
    } else {                         /* n is even and > 2 */
        seq_t A = pow10(n / 2 - 2);
        seq_t B = pow10(n - 3);
        B = (seq_t)11 * B;           /* 11 * 10^(n-3) */
        seq_t diff = B - A;          /* always positive */
        seq_t res = (seq_t)45 * diff;
        return res;
    }
}
