/*
 * A146096  Bell numbers (A000110) read mod 6.
 * https://oeis.org/A146096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n) mod 6. The sequence is periodic with period 39
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t cache[39];
    if (!ready)
    {
        /* binomial coefficients C(i,k) modulo 6 for 0 ≤ i ≤ 38 */
        static seq_t binom[39][39];
        for (idx_t i = 0; i < 39; ++i)
        {
            binom[i][0] = binom[i][i] = 1;
            for (idx_t k = 1; k < i; ++k)
            {
                binom[i][k] = (binom[i - 1][k - 1] + binom[i - 1][k]) % 6;
            }
        }

        /* Bell numbers modulo 6 (Bell[0]=1) up to n=38 */
        static seq_t bell[39];
        bell[0] = 1;
        for (idx_t i = 0; i < 38; ++i)           /* compute Bell[i+1] */
        {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k)
            {
                sum = (sum + binom[i][k] * bell[k]) % 6;
            }
            bell[i + 1] = sum;
        }

        for (idx_t i = 0; i < 39; ++i)
            cache[i] = bell[i];
        ready = true;
    }

    idx_t idx = n % 39;
    return cache[idx];
}
