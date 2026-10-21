/*
 * A080523  a(n) = n^n - n(n-1)/2.
 * https://oeis.org/A080523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - n*(n-1)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(idx_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = (seq_t)base;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t power = pow_seq(n, n);
    seq_t sub = (seq_t)n * ((seq_t)n - 1u) / 2u;
    return power - sub;
}
