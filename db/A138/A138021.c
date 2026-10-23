/*
 * A138021  a(n) = the number of positive divisors k of 2n where |k -2n/k|
 *          divides 2n.
 * https://oeis.org/A138021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive divisors k of 2n such that |k -
 *            (2n/k)| divides 2n
 * Range:     u8 n <= 663, u16 n <= 8399, u32 n <= 8399, u64 n <= 8399,
 *            u128 n <= 8399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h> /* for sqrt if desired */

seq_t a(idx_t n)
{
    /* m = 2 * n */
    seq_t m = (seq_t)n * (seq_t)2;
    seq_t count = 0;

    /* Enumerate divisors up to sqrt(m) */
    for (idx_t i = 1; (idx_t)i * (idx_t)i <= (idx_t)m; ++i)
    {
        if (m % (seq_t)i == 0)
        {
            /* first divisor k = i */
            seq_t k1 = (seq_t)i;
            seq_t q1 = m / k1;                   /* m / k1 */
            seq_t d1 = (k1 > q1) ? (k1 - q1) : (q1 - k1);
            if (d1 > 0 && m % d1 == 0)
                ++count;

            /* second divisor k = q1, distinct from i */
            if (q1 != k1)
            {
                seq_t k2 = q1;
                seq_t q2 = m / k2;                 /* should be i */
                seq_t d2 = (k2 > q2) ? (k2 - q2) : (q2 - k2);
                if (d2 > 0 && m % d2 == 0)
                    ++count;
            }
        }
    }

    return count;
}
