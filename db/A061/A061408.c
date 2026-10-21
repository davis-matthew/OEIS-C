/*
 * A061408  For each y >= 1 there are only finitely many values of x >= 1 such
 *          that x-y and x+y are both positive squares; list all such pairs
 *          (x,y) ordered by values of y; sequence gives y values.
 * https://oeis.org/A061408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061408:  y–values for which there exist positive integers r,k with
 *  y = 2·k·(k+r).  Each y occurs once for every factor pair (k,t) with
 *  k*t = y/2 and k < t.  The terms are output sorted increasingly by y.
 *  The routine counts, for each even y, how many admissible k divide m=y/2
 *  and satisfies k < m/k (i.e., k*k < m).  When the cumulative count reaches
 *  the requested index it returns that y. */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t target = n;
    idx_t pos = 0;
    for (idx_t y = 4; ; y += 2) {          /* only even y can appear    */
        idx_t m = y / 2;                   /* m = y/2                    */
        /* for each divisor k of m with k < m/k count one occurrence of y   */
        for (idx_t k = 1; k * k < m; ++k) { /* k*k < m  ⇔  k < m/k          */
            if (m % k == 0) {
                if (pos == target) {
                    return (seq_t)y;       /* cast to seq_t for final return */
                }
                ++pos;
            }
        }
    }
}
