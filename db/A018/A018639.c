/*
 * A018639  Divisors of 775.
 * https://oeis.org/A018639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 775 in ascending order
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The primes of 775 are 5^2 and 31. */
    static seq_t divisors[6];
    static bool init = false;

    if (!init)
    {
        const seq_t N = (seq_t)775;
        seq_t temp[6];
        size_t count = 0;

        for (seq_t d = 1; d * d <= N; ++d)
        {
            if (N % d == 0)
            {
                temp[count++] = d;
                if (d != N / d)
                    temp[count++] = N / d;
            }
        }

        /* Simple selection sort (6 elements). */
        for (size_t i = 0; i < count; ++i)
        {
            size_t min = i;
            for (size_t j = i + 1; j < count; ++j)
            {
                if (temp[j] < temp[min])
                    min = j;
            }
            seq_t t = temp[i];
            temp[i] = temp[min];
            temp[min] = t;
        }

        for (size_t i = 0; i < count; ++i)
            divisors[i] = temp[i];

        init = true;
    }

    if (n >= 1 && n <= 6)
        return divisors[(size_t)(n - 1)];
    else
        return (seq_t)0;
}
