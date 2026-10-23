/*
 * A108412  Expansion of (1 + x + x^2)/(1 - 4x^2 + x^4).
 * https://oeis.org/A108412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-4) with a(0)=1, a(1)=1, a(2)=5, a(3)=4
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   mul_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_safe.h"

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return 1;
    if (n == 1u) return 1;
    if (n == 2u) return 5;
    if (n == 3u) return 4;

    /* Sliding window: a0=a(i-4), a1=a(i-3), a2=a(i-2), a3=a(i-1) */
    seq_t a0 = 1;   /* a(i-4) */
    seq_t a1 = 1;   /* a(i-3) */
    seq_t a2 = 5;   /* a(i-2) */
    seq_t a3 = 4;   /* a(i-1) */
    seq_t new_val;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 4*a(i-2) - a(i-4) */
        new_val = mul_safe(a2, 4u) - a0;

        /* Shift the window for the next step */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }

    return a3;  /* a(n) is stored in a3 after the loop */
}
