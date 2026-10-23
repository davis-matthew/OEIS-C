/*
 * A018314  Divisors of 168.
 * https://oeis.org/A018314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest divisor of 168 (sorted ascending)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[16];
    static int initialized = 0;
    if (!initialized)
    {
        size_t idx = 0;
        for (idx_t e2 = 0; e2 <= 3; ++e2)
        {
            seq_t two = (seq_t)1 << e2;
            for (idx_t e3 = 0; e3 <= 1; ++e3)
            {
                seq_t v3 = (seq_t)1;
                for (idx_t i = 0; i < e3; ++i) v3 *= (seq_t)3;
                seq_t v2 = two * v3;
                for (idx_t e7 = 0; e7 <= 1; ++e7)
                {
                    seq_t v7 = (seq_t)1;
                    for (idx_t i = 0; i < e7; ++i) v7 *= (seq_t)7;
                    divs[idx++] = v2 * v7;
                }
            }
        }
        /* sort ascending */
        for (size_t i = 0; i < 16 - 1; ++i)
            for (size_t j = i + 1; j < 16; ++j)
                if (divs[j] < divs[i])
                {
                    seq_t t = divs[i];
                    divs[i] = divs[j];
                    divs[j] = t;
                }
        initialized = 1;
    }
    if (n < 1 || (size_t)n > 16)
        return (seq_t)0;
    return divs[(size_t)n - 1];
}
