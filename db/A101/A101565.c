/*
 * A101565  a(n) = Sum_{k=0..n} (-1)^A010060(n-k)*[(n+1) mod (k+1) = 0], where
 *          [] denotes the Iverson bracket.
 * https://oeis.org/A101565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^A010060(n-k)*[(n+1) mod (k+1) = 0]
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   parity_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity_uint64.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;          /* m = n + 1 */
    seq_t sum = 0;

    for (idx_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            /* divisor d1 = i */
            idx_t d1 = i;
            idx_t x = m - d1;
            seq_t sign = parity_uint64(x) ? (seq_t)-1 : (seq_t)1;
            sum += sign;

            /* divisor d2 = m / i, if distinct */
            idx_t d2 = m / i;
            if (d2 != d1)
            {
                x = m - d2;
                sign = parity_uint64(x) ? (seq_t)-1 : (seq_t)1;
                sum += sign;
            }
        }
    }

    return sum;
}
