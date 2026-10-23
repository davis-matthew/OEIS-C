/*
 * A041676  Numerators of continued fraction convergents to sqrt(357).
 * https://oeis.org/A041676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(357) Computed by
 *            the standard recurrence p(-2)=0, p(-1)=1 p(n) = a(n)*p(n-1) +
 *            p(n-2) where a(0)=18 and for n>0 the continued‑fraction digits
 *            repeat 1, 8, 2, 8, 1, 36
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for p(-2) and p(-1) */
    seq_t p_minus2 = (seq_t)0;
    seq_t p_minus1 = (seq_t)1;

    /* Cycle of continued–fraction digits for sqrt(357). */
    static const seq_t cycle[6] = {(seq_t)1, (seq_t)8, (seq_t)2,
                                   (seq_t)8, (seq_t)1, (seq_t)36};

    seq_t cur = (seq_t)0;
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        seq_t a_i;
        if (i == (idx_t)0) {
            a_i = (seq_t)18;
        } else {
            /* (i-1) % 6 selects the appropriate cycle element. */
            a_i = cycle[(i - (idx_t)1) % (idx_t)6];
        }
        cur = a_i * p_minus1 + p_minus2;
        /* Prepare values for the next iteration. */
        p_minus2 = p_minus1;
        p_minus1 = cur;
    }
    return cur;
}
