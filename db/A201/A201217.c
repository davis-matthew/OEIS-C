/*
 * A201217  Numbers such that (closest square) = (closest cube).
 * https://oeis.org/A201217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 883, u32 n <= 6051, u64 n <= 6051,
 *            u128 n <= 6051, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers such that (closest square) = (closest cube).
   Computed by recognising that for each p≥1 the sequence contains all
   integers in the interval [p^6 - p^3 + 1, p^6 + p^3].  The value a(n) is
   obtained by determining the appropriate p from cumulative terms
   (1 + 2∑_{k=1}^{p-1}k^3) and then the offset inside that interval. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 is a special case */
    if (n == 1) return 0;

    idx_t cum = 1;    /* count of terms up to previous p (including a(1)) */
    idx_t p   = 1;    /* current p */

    /* find p such that cum + 2*p^3 >= n */
    while (cum + 2 * p * p * p < n) {
        cum += 2 * p * p * p;
        ++p;
    }

    idx_t r = n - cum;                 /* 1‑based index within block p  */
    seq_t p3 = (seq_t)p * p * p;       /* p^3   */
    seq_t p6 = p3 * p3;                /* p^6   */
    seq_t val = p6 - p3 + (seq_t)r;    /* a(n)  */

    return val;
}
