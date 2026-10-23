/*
 * A018211  Alkane (or paraffin) numbers l(10,n).
 * https://oeis.org/A018211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+7,7)/2 + ((1+(-1)^n)*(n+2)(n+4)(n+6))/192
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute binom(n+7,7) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 bin = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        bin = bin * (unsigned __int128)(n + i) / i;   /* exact division */
    }

    /* a = [96*bin + (if n even? 2*P : 0)] / 192   where P = (n+2)(n+4)(n+6) */
    unsigned __int128 num = 96 * bin;

    if ((n & 1) == 0) {               /* n even */
        unsigned __int128 p = (unsigned __int128)(n + 2) *
                             (n + 4) *
                             (n + 6);
        num += 2 * p;
    }

    return (seq_t)(num / 192);        /* divide by 192, exact integer result */
}
