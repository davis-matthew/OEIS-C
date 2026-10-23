/*
 * A018634  Divisors of 765.
 * https://oeis.org/A018634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 765 sorted in increasing order. */
#include <stdbool.h>

/* Compute the nth divisor of 765.  For n > 12 the function returns 0. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[12];

    if (!initialized)
    {
        int pos = 0;
        seq_t pow3 = 1;
        for (int e3 = 0; e3 <= 2; ++e3)
        {
            seq_t pow5 = 1;
            for (int e5 = 0; e5 <= 1; ++e5)
            {
                seq_t pow17 = 1;
                for (int e17 = 0; e17 <= 1; ++e17)
                {
                    divisors[pos++] = pow3 * pow5 * pow17;
                    pow17 *= (seq_t)17;
                }
                pow5 *= (seq_t)5;
            }
            pow3 *= (seq_t)3;
        }

        /* Sort the 12 values in ascending order. */
        for (int i = 0; i < 12; ++i)
        {
            for (int j = i + 1; j < 12; ++j)
            {
                if (divisors[i] > divisors[j])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        initialized = true;
    }

    if (n >= 1 && n <= 12)
        return divisors[n - 1];
    else
        return (seq_t)0;   /* undefined for n > 12 */
}
