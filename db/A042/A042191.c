/*
 * A042191  Denominators of continued fraction convergents to sqrt(620).
 * https://oeis.org/A042191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042191: Denominators of continued‑fraction convergents to sqrt(620)
   Recurrence:  a(0..7) are the initial values below,
   a(n) = 498*a(n-4) - a(n-8)    for n ≥ 8   */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0 .. 7 */
    static const seq_t init[8] = {
        (seq_t)1,    /* a(0) */
        (seq_t)1,    /* a(1) */
        (seq_t)9,    /* a(2) */
        (seq_t)10,   /* a(3) */
        (seq_t)489,  /* a(4) */
        (seq_t)499,  /* a(5) */
        (seq_t)4481, /* a(6) */
        (seq_t)4980  /* a(7) */
    };

    /* For the first 8 indices return the stored values */
    if (n < 8)
        return init[n];

    /* Keep the last eight computed terms in a circular buffer */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* Compute terms from 8 up to n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = (seq_t)498 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = val;
    }

    /* buf[n & 7] now holds a(n) */
    return buf[n & 7];
}
