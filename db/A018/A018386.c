/*
 * A018386  Divisors of 310.
 * https://oeis.org/A018386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 310 in increasing order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool initialized = false;
    if (!initialized)
    {
        size_t idx = 0;
        for (unsigned e2 = 0; e2 <= 1; ++e2)
        {
            for (unsigned e5 = 0; e5 <= 1; ++e5)
            {
                for (unsigned e31 = 0; e31 <= 1; ++e31)
                {
                    seq_t v = 1;
                    if (e2) v *= (seq_t)2;
                    if (e5) v *= (seq_t)5;
                    if (e31) v *= (seq_t)31;
                    divisors[idx++] = v;
                }
            }
        }
        // simple bubble sort
        for (size_t i = 0; i < 7; ++i)
        {
            for (size_t j = i + 1; j < 8; ++j)
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
    if (n >= 1 && n <= 8)
        return divisors[n - 1];
    return (seq_t)0;
}
