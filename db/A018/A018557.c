/*
 * A018557  Divisors of 627.
 * https://oeis.org/A018557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 627 (sorted ascending)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The integer 627 has exactly 8 divisors. Compute them once,
       cache the result, and return the requested element. */
    static seq_t divisors[8];
    static idx_t div_cnt = 0U;

    if (div_cnt == 0U)
    {
        const idx_t LIMIT = 627U;
        for (idx_t i = 1U; i <= LIMIT; ++i)
        {
            if (LIMIT % i == 0U)
            {
                divisors[div_cnt++] = (seq_t)i;
            }
        }
    }

    if (n >= 1U && n <= div_cnt)
        return divisors[n - 1U];

    /* Undefined for n > 8; return 0 as a sentinel. */
    return (seq_t)0;
}
