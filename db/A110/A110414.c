/*
 * A110414  n! concatenated with n divided by n.
 * https://oeis.org/A110414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n-1)! * 10^d + 1, where d = number of decimal digits of
 *            n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for computing 10^exp */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* for n>=1 */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* number of decimal digits in n */
    idx_t d = 1;
    idx_t t = n;
    while (t >= 10) {
        t /= 10;
        ++d;
    }

    seq_t pow10d = pow10(d);
    return fact * pow10d + 1;
}
