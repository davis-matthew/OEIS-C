/*
 * A235861  Regular continued fraction expansion of square root of 4729494.
 * https://oeis.org/A235861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0)=2174; a(n)=a(n-92) for n>92. The periodic part
 *            (a(1)..a(92)) * is the regular continued‑fraction expansion of
 *            sqrt(4729494)
 * Range:     u8 none, u16 none, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Even though the sequence is periodic with period 92, compute the
     * period only once and cache it. */
    static seq_t period[92];
    static bool computed = false;

    if (!computed) {
        /* Pre‑computations for sqrt(4729494). */
        const seq_t sqrt_floor = (seq_t)2174;      /* a(0)            */
        const seq_t D           = (seq_t)4729494;  /* the radicand     */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_n = sqrt_floor;   /* a(0) for the algorithm */

        /* Compute the first 92 terms of the period (a(1)..a(92)). */
        for (int i = 0; i < 92; ++i) {
            m = d * a_n - m;
            d = (D - m * m) / d;
            a_n = (sqrt_floor + m) / d;
            period[i] = a_n;
        }
        computed = true;
    }

    if (n == 0u) {
        return (seq_t)2174;
    } else {
        /* The period starts at index 1; use (n-1) mod 92. */
        idx_t mod = (n - 1u) % (idx_t)92;
        return period[mod];
    }
}
