/*
 * A000266  Expansion of e.g.f. exp(-x^2/2) / (1-x).
 * https://oeis.org/A000266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) for odd n; a(n) = n*a(n-1) + (-1)^(n/2) *
 *            (n-1)!! for even n; a(0) = 1. Derived from a(n) = n! *
 *            Sum_{i=0..floor(n/2)} (-1)^i / (i! * 2^i), using n!/((n/2)! *
 *            2^(n/2)) = (n-1)!!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;   /* a(k) */
    seq_t df = 1;  /* (largest odd <= k)!! */
    idx_t k;
    for (k = 1; k <= n; k++) {
        r = r * (seq_t)k;
        if (k & 1) {
            df = df * (seq_t)k;
        } else {
            if ((k / 2) & 1)
                r -= df;
            else
                r += df;
        }
    }
    return r;
}
