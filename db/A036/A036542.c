/*
 * A036542  a(n) = T(n,n), array T given by A047858.
 * https://oeis.org/A036542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*2^(n-1) - n + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow2_seq(idx_t exp) {
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        r = r * (seq_t)2;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t pow2 = pow2_seq(n - 1);
    seq_t term = nn * pow2;
    term = term * (seq_t)3;
    term = term - nn;
    term = term + (seq_t)1;
    return term;
}
