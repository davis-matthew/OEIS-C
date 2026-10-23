/*
 * A128060  a(n) = 2*n - numerator((2*n-1)^2/(2*(2*n)!)).
 * https://oeis.org/A128060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*n - A128059(n) = gcd(2*n-1, (2*n-2)!) for n>0, a(0) =
 *            -1. Since 2*n-1 is odd, gcd equals 2*n-1 if it is composite,
 *            otherwise 1
 * Range:     i8 n <= 64, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;          /* a(0) = -1 */

    seq_t x = (seq_t)n * 2 - 1;   /* 2*n-1 */

    if (is_prime_small(x))
        return 1;                 /* prime case */
    else
        return x;                 /* composite case */
}
