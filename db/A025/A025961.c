/*
 * A025961  Expansion of 1/((1-2*x)*(1-4*x)*(1-5*x)*(1-9*x)).
 * https://oeis.org/A025961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 137*a(n-2) + 382*a(n-3) - 360*a(n-4) with
 *            initial values a(0)=1, a(1)=20, a(2)=263, a(3)=2902
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Directly handle the first four terms. */
    if (n == 0) return 1;
    if (n == 1) return 20;
    if (n == 2) return 263;
    if (n == 3) return 2902;

    seq_t a0 = 1, a1 = 20, a2 = 263, a3 = 2902, an;
    for (idx_t i = 4; i <= n; ++i) {
        an = 20 * a3 - 137 * a2 + 382 * a1 - 360 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
