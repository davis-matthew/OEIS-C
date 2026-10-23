/*
 * A018298  Divisors of 135.
 * https://oeis.org/A018298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 135 in increasing order
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t TARGET = (seq_t)135;
    idx_t cnt = 0;
    for (seq_t i = 1; i <= TARGET; ++i)
    {
        if (TARGET % i == 0)
        {
            ++cnt;
            if ((idx_t)cnt == n)
                return i;
        }
    }
    return (seq_t)0;
}
