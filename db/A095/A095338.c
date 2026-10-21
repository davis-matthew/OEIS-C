/*
 * A095338  Total number of leaves in the labeled graphs of order n.
 * https://oeis.org/A095338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*2^((n-1)(n-2)/2) (OEIS A095338)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^e for an unsigned exponent e using repeated squaring. */
static seq_t pow2(idx_t e) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e > 0) {
        if (e & 1u) {
            result = result * base;
        }
        /* Avoid one extra multiplication when e becomes 1 */
        if (e > 1u) {
            base = base * base;
        }
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1u) {
        return (seq_t)0;
    }
    /* n*(n-1) fits in seq_t; for builtin types the cast ensures correct type */
    seq_t pref = (seq_t)n * (seq_t)(n - 1u);
    /* exponent e = C(n-1,2) = (n-1)(n-2)/2 */
    idx_t e = (n - 1u) * (n - 2u) / 2u;
    return pref * pow2(e);
}
