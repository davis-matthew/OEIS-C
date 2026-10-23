/*
 * A018531  Divisors of 582.
 * https://oeis.org/A018531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 582, sorted increasingly. For n > 8 the
 *            function returns 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static size_t divcnt = 0;
    static bool initialized = false;

    if (!initialized)
    {
        const idx_t target = 582;          /* 582 = 2 * 3 * 97 */
        idx_t i;

        /* Collect small divisors in ascending order and large ones in a buffer. */
        idx_t big[8];
        size_t bigcnt = 0;

        for (i = 1; i * i <= target; ++i)
        {
            if (target % i == 0)
            {
                divisors[divcnt++] = (seq_t)i;          /* small divisor */
                if (i * i < target)                    /* distinct large divisor */
                {
                    big[bigcnt++] = target / i;
                }
            }
        }

        /* Append the large divisors in ascending order (reverse of how we collected them). */
        for (idx_t k = 0; k < bigcnt; ++k)
        {
            divisors[divcnt++] = (seq_t)big[bigcnt - 1 - k];
        }

        initialized = true;
    }

    if (n == 0 || n > divcnt)
        return (seq_t)0;
    return divisors[n - 1];
}
