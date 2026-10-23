/*
 * A146535  Numerator of (2*n-1)/3.
 * https://oeis.org/A146535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A146535 – Numerator of (2*n‑1)/3
   Formula: a(n) = 2*a(n-3) - a(n-6)   for n ≥ 7
   Initial terms: a(1)=1, a(2)=1, a(3)=5, a(4)=7, a(5)=3, a(6)=11
   (The recurrence matches the linear recurrence with characteristic
   polynomial (x³-1)².) */

#include <stddef.h>   /* for size_t, NULL */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0, not used */

    /* handle the first six terms directly */
    if (n <= 6) {
        static const seq_t init[6] = { 1, 1, 5, 7, 3, 11 };
        return init[n - 1];
    }

    /* circular buffer that always holds the last six values:
          buf[0] = a(i-5), buf[1] = a(i-4), … , buf[5] = a(i-1)   */
    seq_t buf[6] = { 1, 1, 5, 7, 3, 11 };

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 2*a(i-3) - a(i-6) ->
                 a(i-3) is buf[3], a(i-6) is buf[0]        */
        seq_t val = 2 * buf[3] - buf[0];

        /* shift buffer left by one (buf[1] becomes the new buf[0], …) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }

    /* after the loop buf[5] holds a(n) */
    return buf[5];
}
