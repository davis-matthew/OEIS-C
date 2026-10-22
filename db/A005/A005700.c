/*
 * A005700  a(n) = C(n)*C(n+2) - C(n+1)^2 where C() are the Catalan numbers
 *          A000108.
 * https://oeis.org/A005700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = C(n)*C(n+2) - C(n+1)^2, where C() are Catalan numbers
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   binom_2n_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_2n_n.h"

static seq_t catalan(idx_t n) {
    if (n == 0) return 1;
    seq_t b = binom_2n_n(n);
    return b / ((seq_t)n + 1);
}

seq_t a(idx_t n) {
    seq_t c_n = catalan(n);
    seq_t c_n1 = catalan(n + 1);
    seq_t c_n2 = catalan(n + 2);
    
    return c_n * c_n2 - c_n1 * c_n1;
}
