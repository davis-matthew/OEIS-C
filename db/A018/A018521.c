/*
 * A018521  Divisors of 564.
 * https://oeis.org/A018521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 564 in ascending order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Factorization: 564 = 2^2 * 3^1 * 47^1 */
    static seq_t divisors[12];
    static bool initialized = false;
    if (!initialized)
    {
        int count = 0;
        /* generate all divisors */
        for (int e0 = 0; e0 <= 2; ++e0)
        {
            seq_t p0 = 1;
            for (int i = 0; i < e0; ++i) p0 *= (seq_t)2;
            for (int e1 = 0; e1 <= 1; ++e1)
            {
                seq_t p1 = p0;
                for (int i = 0; i < e1; ++i) p1 *= (seq_t)3;
                for (int e2 = 0; e2 <= 1; ++e2)
                {
                    seq_t v = p1;
                    for (int i = 0; i < e2; ++i) v *= (seq_t)47;
                    divisors[count++] = v;
                }
            }
        }
        /* sort the 12 divisors in ascending order */
        for (int i = 0; i < 11; ++i)
            for (int j = i + 1; j < 12; ++j)
                if (divisors[j] < divisors[i])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        initialized = true;
    }

    if (n == 0) return (seq_t)0;              /* undefined index */
    idx_t idx = n - 1;                         /* convert to 0‑based */
    if (idx >= 12) return (seq_t)0;            /* out of range */
    return divisors[idx];
}
