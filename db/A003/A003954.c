/*
 * A003954  Expansion of g.f.: (1+x)/(1-11*x).
 * https://oeis.org/A003954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 12 * 11^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    
    seq_t power = a000272_ipow((seq_t)11, n - 1);
    return (seq_t)12 * power;
}
