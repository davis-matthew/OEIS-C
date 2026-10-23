/*
 * A018663  Divisors of 819.
 * https://oeis.org/A018663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 819 (OEIS A018663) */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If the divisor list is not yet built, build it once. */
    static bool init = false;
    static seq_t divisors[12];
    static idx_t count = 0;
    const seq_t N = (seq_t)819;  /* The number whose divisors we list */

    if (!init)
    {
        /* Temporary arrays to store small and large divisors. */
        seq_t small[12];
        seq_t large[12];
        idx_t small_n = 0, large_n = 0;

        for (idx_t i = 1; (seq_t)i * (seq_t)i <= N; ++i)
        {
            if (N % (seq_t)i == 0)
            {
                small[small_n++] = (seq_t)i;
                seq_t j = N / (seq_t)i;
                if (j != (seq_t)i)
                    large[large_n++] = j;
            }
        }

        /* Combine small divisors (ascending) and large divisors (descending). */
        idx_t idx = 0;
        for (idx_t i = 0; i < small_n; ++i)
            divisors[idx++] = small[i];
        for (idx_t i = large_n; i-- > 0; )
            divisors[idx++] = large[i];

        count = idx;
        init = true;
    }

    /* Return the nth divisor if it exists, otherwise 0. */
    if (n >= 1 && n <= count)
        return divisors[n - 1];
    return (seq_t)0;
}
