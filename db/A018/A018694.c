/*
 * A018694  Divisors of 872.
 * https://oeis.org/A018694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 872: a(n) is the n‑th smallest divisor of 872 (0 for n>8) */

#include <stdlib.h>
#include <stdbool.h>

/* The function to compute the n-th divisor of 872. */
seq_t a(idx_t n)
{
    /* The number 872 = 2^3 * 109.  There are only 8 divisors,
       so we compute them once and reuse. */
    static seq_t divisors[8];
    static bool initialized = false;
    if (!initialized)
    {
        /* Generate all divisors by trial division. */
        size_t cnt = 0;
        for (idx_t d = 1; d * d <= 872; ++d)
        {
            if (872 % d == 0)
            {
                /* First divisor */
                divisors[cnt++] = (seq_t)d;
                /* Complement divisor, avoid duplicate for perfect square */
                if (d != 872 / d)
                {
                    divisors[cnt++] = (seq_t)(872 / d);
                }
            }
        }
        /* Simple insertion sort to order the divisors ascending. */
        for (size_t i = 1; i < cnt; ++i)
        {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key)
            {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n == 0)
        return 0;
    size_t pos = (size_t)(n - 1);
    if (pos < 8)
        return divisors[pos];
    /* For indices beyond the number of divisors, return 0. */
    return 0;
}
