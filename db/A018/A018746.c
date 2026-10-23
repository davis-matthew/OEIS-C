/*
 * A018746  Divisors of 963.
 * https://oeis.org/A018746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 963 in increasing order (if n
 *            exceeds the number of divisors, a(n) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the prime factorization of 963 is 3^2 * 107^1 */
    static seq_t divisors[6];
    static bool initialized = false;

    if (!initialized)
    {
        /* generate all combinations of exponents */
        seq_t val = 1;
        int pos = 0;
        for (int exp3 = 0; exp3 <= 2; ++exp3)
        {
            for (int exp107 = 0; exp107 <= 1; ++exp107)
            {
                /* compute 3^exp3 * 107^exp107 */
                seq_t v = 1;
                for (int i = 0; i < exp3; ++i) v *= 3;
                for (int i = 0; i < exp107; ++i) v *= 107;
                divisors[pos++] = v;
            }
        }
        /* sort the 6 divisors ascending (simple insertion sort) */
        for (int i = 1; i < 6; ++i)
        {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key)
            {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        initialized = true;
    }

    if (n == 0)
        return 0; /* undefined index, return 0 */

    if (n >= 1 && n <= 6)
        return divisors[n - 1];
    else
        return 0; /* no divisor, return 0 for indices beyond the list */
}
