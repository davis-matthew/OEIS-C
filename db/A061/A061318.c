/*
 * A061318  Column 3 of A061314.
 * https://oeis.org/A061318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0..8) = {0,4,40,304,1720,7480,26404,79240,209304}
   For n ≥ 9:
     a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
            + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
   (These coefficients come from the generating function
    4*x*(1+x+22*x²+22*x³+22*x⁴+x⁵+x⁶)/(1-x)⁹. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed first nine terms (a(0) … a(8)). */
    static const seq_t init[9] = {
        (seq_t)0,
        (seq_t)4,
        (seq_t)40,
        (seq_t)304,
        (seq_t)1720,
        (seq_t)7480,
        (seq_t)26404,
        (seq_t)79240,
        (seq_t)209304
    };

    if (n < 9)
        return init[n];

    /* Initialise a sliding window of the last nine values. */
    seq_t v0 = init[0], v1 = init[1], v2 = init[2];
    seq_t v3 = init[3], v4 = init[4], v5 = init[5];
    seq_t v6 = init[6], v7 = init[7], v8 = init[8];
    seq_t next = 0;

    for (idx_t i = 9; i <= n; ++i) {
        /* Compute a(i) via the recurrence. */
        next = 9*v8 - 36*v7 + 84*v6 - 126*v5
               + 126*v4 - 84*v3 + 36*v2 - 9*v1 + v0;

        /* Slide the window for the next iteration. */
        v0 = v1; v1 = v2; v2 = v3; v3 = v4; v4 = v5;
        v5 = v6; v6 = v7; v7 = v8; v8 = next;
    }

    return v8; /* a(n) */
}
