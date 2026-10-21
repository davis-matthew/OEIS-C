/*
 * A212251  Number of (w,x,y,z) with all terms in {1,...,n} and 3w = x + y + z
 *          + n + 1.
 * https://oeis.org/A212251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6), a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 6, a(4) = 15, a(5)
 *            = 30
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t base[6] = {0, 0, 1, 6, 15, 30};

    if (n < 6)
        return base[n];

    /* last six values of the sequence */
    seq_t prev[6];
    for (int i = 0; i < 6; ++i)
        prev[i] = base[i];

    seq_t newval = 0;
    for (idx_t i = 6; i <= n; ++i) {
        /* compute using signed arithmetic to avoid unsigned wrap‑around */
        int64_t t = 0;
        t += 3 * (int64_t)prev[5];
        t -= 3 * (int64_t)prev[4];
        t += 2 * (int64_t)prev[3];
        t -= 3 * (int64_t)prev[2];
        t += 3 * (int64_t)prev[1];
        t -= (int64_t)prev[0];
        newval = (seq_t)t;

        /* shift values for next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = newval;
    }
    return newval;
}
