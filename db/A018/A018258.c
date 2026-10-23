/*
 * A018258  Divisors of 42.
 * https://oeis.org/A018258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 42 in ascending order (1‑indexed)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices outside the defined range */
    if (n < 1) return (seq_t)0;
    if (n > 8) return (seq_t)0;

    /* Lazily generate and cache the 8 divisors of 42 */
    static int initialized = 0;
    static seq_t divisors[8];
    if (!initialized)
    {
        int idx = 0;
        for (int i = 0; i <= 1; ++i)
        {
            seq_t p2 = (i == 0) ? (seq_t)1 : (seq_t)2;
            for (int j = 0; j <= 1; ++j)
            {
                seq_t p3 = (j == 0) ? (seq_t)1 : (seq_t)3;
                for (int k = 0; k <= 1; ++k)
                {
                    seq_t p7 = (k == 0) ? (seq_t)1 : (seq_t)7;
                    divisors[idx++] = p2 * p3 * p7;
                }
            }
        }
        /* Simple insertion sort – O(8²) negligible */
        for (int i = 0; i < 7; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (divisors[i] > divisors[j])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        initialized = 1;
    }

    return divisors[n - 1];
}
