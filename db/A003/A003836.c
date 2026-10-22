/*
 * A003836  Order of universal Chevalley group D_n (9).
 * https://oeis.org/A003836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(9,n) = 9^(n*(n-1)) * (9^n - 1) * Product_{i=1..n-1}
 *            (9^(2*i) - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e > 0) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t p = 1;
    idx_t i;
    if (n == 0) return 0;
    for (i = 1; i < n; i++) {
        p *= 81;
        result *= (p - 1);
    }
    result *= (pow_seq((seq_t)9, n) - 1);
    result *= pow_seq((seq_t)9, n * (n - 1));
    return result;
}
