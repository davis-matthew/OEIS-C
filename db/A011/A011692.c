/*
 * A011692  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^6+x^4+x^3+x^2+x+1.
 * https://oeis.org/A011692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binary m‑sequence defined by the reciprocal of x^8 + x^6
 *            + x^4 + x^3 + x^2 + x + 1 over GF(2). The sequence has period
 *            255 and satisfies a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1,
 *            and for n ≥ 8: a(n) = a(n-1) ⊕ a(n-2) ⊕ a(n-3) ⊕ a(n-4) ⊕ a(n-6)
 *            ⊕ a(n-8)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PERIOD 255

seq_t a(idx_t n)
{
    static seq_t period[PERIOD];
    static bool initialized = false;

    if (!initialized)
    {
        /* Explicitly set the first eight values */
        period[0] = 0; period[1] = 0; period[2] = 0; period[3] = 0;
        period[4] = 0; period[5] = 0; period[6] = 0; period[7] = 1;

        /* Generate the remaining 247 terms using the recurrence */
        idx_t i;
        for (i = 8; i < PERIOD; ++i)
        {
            period[i]  = period[i-1] ^ period[i-2] ^ period[i-3];
            period[i] ^= period[i-4] ^ period[i-6] ^ period[i-8];
        }
        initialized = true;
    }

    return period[n % PERIOD];
}
