/*
 * A083319  a(n) = 4^n+3^n-2^n.
 * https://oeis.org/A083319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 3^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* seq_t and idx_t are defined by the build; do not redefine them */

static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n);
    return pow4 + pow3 - pow2;
}
