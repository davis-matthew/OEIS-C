/*
 * A188589  Expansion of (1-3*x+6*x^2-3*x^3)/((1-x)^2*(1-2*x)).
 * https://oeis.org/A188589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=5*2^(n-1)-n-3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t term = (seq_t)5 * pow2(n - 1) - (seq_t)n - (seq_t)3;
    return term;
}
