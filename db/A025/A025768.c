/*
 * A025768  Expansion of 1/((1-x)*(1-x^3)*(1-x^7)).
 * https://oeis.org/A025768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n+3)*(n+8)/42)
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t prod = (nn + (seq_t)3) * (nn + (seq_t)8);
    seq_t val  = (prod + (seq_t)21) / (seq_t)42;
    return val;
}
