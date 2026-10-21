/*
 * A211482  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w*x+w*y+x*y=w*x*y.
 * https://oeis.org/A211482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*n + 8 for n > 5 Initial values: a(0)=1, a(1)=10,
 *            a(2)=22, a(3)=35, a(4)=50, a(5)=62 This implements the OEIS
 *            sequence A211482
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 22;
    if (n == 3) return 35;
    if (n == 4) return 50;
    if (n == 5) return 62;
    /* n > 5 */
    return (seq_t)12 * (seq_t)n + (seq_t)8;
}
