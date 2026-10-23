/*
 * A018446  Divisors of 424.
 * https://oeis.org/A018446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 424 in ascending order */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* If n < 1 or n > 8 the sequence is not defined.  Return 0. */
    if (n < 1 || n > 8) return 0;

    /* Build the divisors of 424 once.  424 = 2^3 * 53. */
    static seq_t divisors[8];
    static bool ready = false;
    if (!ready)
    {
        int i = 0;
        for (int e2 = 0; e2 <= 3; ++e2)
        {
            seq_t v2 = 1;
            for (int k = 0; k < e2; ++k) v2 *= 2;          /* 2^e2 */
            for (int e53 = 0; e53 <= 1; ++e53)
            {
                seq_t val = v2;
                if (e53) val *= 53;                       /* multiply by 53 if needed */
                divisors[i++] = val;
            }
        }
        /* Sort the 8 divisors ascending. */
        for (int i = 0; i < 8 - 1; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (divisors[i] > divisors[j])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        ready = true;
    }

    return divisors[(idx_t)(n - 1)];
}
