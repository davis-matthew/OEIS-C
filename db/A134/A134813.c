/*
 * A134813  a(n) = b(n+1)-2b(n) where b() is A134812.
 * https://oeis.org/A134813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^k * 6 * 4^(k-1) for n = 4k a(n) = (-1)^(k+1) * 3 *
 *            4^k for n = 4k+1 a(n) = (-1)^k * 3 * 4^k for n = 4k+2 a(n) = 0
 *            for n = 4k+3 where k = floor(n/4). The special case a(0) = 1 is
 *            handled separately
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* special initial term */
        return 1;

    idx_t mod = n & 3;     /* n % 4 */
    idx_t k   = n >> 2;    /* floor(n/4) */

    /* compute 4^k */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow4 *= 4;

    seq_t result;
    switch (mod) {
    case 0:                 /* n = 4k, k >= 1 because n != 0 */
        pow4 /= 4;                /* 4^(k-1) */
        result = pow4 * 6;
        if (k & 1)                /* (-1)^k */
            result = -result;
        break;
    case 1:                 /* n = 4k+1 */
        result = pow4 * 3;
        if (((k + 1) & 1))        /* (-1)^(k+1) */
            result = -result;
        break;
    case 2:                 /* n = 4k+2 */
        result = pow4 * 3;
        if (k & 1)                /* (-1)^k */
            result = -result;
        break;
    case 3:                 /* n = 4k+3 */
        result = 0;
        break;
    }
    return result;
}
