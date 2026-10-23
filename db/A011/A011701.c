/*
 * A011701  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^5+x^4+x^3+x^2+x+1.
 * https://oeis.org/A011701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) ⊕ a(n-2) ⊕ a(n-3) ⊕ a(n-4) ⊕ a(n-5) ⊕ a(n-8) (mod
 *            2) with initial values a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0,
 *            a(7)=1. The sequence is 85‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* precomputed period of length 85 (mod 2) */
    static seq_t period[85];
    static int initialized = 0;

    if (!initialized) {
        /* initial seven zeros and a single one at position 7 */
        period[0] = 0; period[1] = 0; period[2] = 0; period[3] = 0;
        period[4] = 0; period[5] = 0; period[6] = 0; period[7] = 1;

        /* compute remaining terms using the recurrence */
        for (size_t i = 8; i < 85; ++i) {
            period[i] =
                period[i - 1] ^ period[i - 2] ^ period[i - 3] ^
                period[i - 4] ^ period[i - 5] ^ period[i - 8];
        }
        initialized = 1;
    }

    /* use the 85‑cycle to get any term */
    size_t idx = (size_t)n % 85;
    return period[idx];
}
