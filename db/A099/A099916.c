/*
 * A099916  Expansion of (1+x^2)^2/(1-x^3+x^6).
 * https://oeis.org/A099916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-3) - a(n-6) + p(n), where p(n)=1 if n=0, 2 if n=2, 1
 *            if n=4, 0 otherwise. This is derived from (1+x^2)^2/(1-x^3+x^6)
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Circular buffer for the last 6 terms of the sequence. */
    seq_t ring[6] = {0, 0, 0, 0, 0, 0};
    seq_t val, term_p;
    for (idx_t i = 0; i <= n; ++i) {
        /* Determine p(i). */
        switch (i) {
            case 0: term_p = 1; break;
            case 2: term_p = 2; break;
            case 4: term_p = 1; break;
            default: term_p = 0; break;
        }
        val = term_p;
        if (i >= 3) {
            val += ring[(i - 3) % 6];
        }
        if (i >= 6) {
            val -= ring[(i - 6) % 6];
        }
        ring[i % 6] = val;   /* store current term */
    }
    return ring[n % 6];
}
