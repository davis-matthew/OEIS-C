/*
 * A018685  Divisors of 855.
 * https://oeis.org/A018685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 855 (0 for n > 12)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Number of divisors of 855 is fixed (12). Compute on first call. */
    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized)
    {
        idx_t count = 0;
        /* 855 = 5^1 * 3^2 * 19^1 */
        for (int e5 = 0; e5 <= 1; ++e5)
        {
            seq_t pow5 = 1;
            for (int i = 0; i < e5; ++i)
                pow5 *= (seq_t)5;

            for (int e3 = 0; e3 <= 2; ++e3)
            {
                seq_t pow3 = 1;
                for (int i = 0; i < e3; ++i)
                    pow3 *= (seq_t)3;

                for (int e19 = 0; e19 <= 1; ++e19)
                {
                    seq_t pow19 = 1;
                    for (int i = 0; i < e19; ++i)
                        pow19 *= (seq_t)19;

                    divisors[count++] = pow5 * pow3 * pow19;
                }
            }
        }

        /* Simple insertion sort: 12 elements only. */
        for (idx_t i = 1; i < count; ++i)
        {
            seq_t key = divisors[i];
            long long j = (long long)i - 1;
            while (j >= 0 && divisors[j] > key)
            {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        initialized = true;
    }

    if (n == 0 || n > 12)
        return (seq_t)0;
    return divisors[n - 1];
}
