/*
 * A003930  Order of simple Chevalley group B_n (9).
 * https://oeis.org/A003930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^(n^2) * Product_{i=1..n} (9^(2*i) - 1) / 2, computed as
 *            9^(n^2) * 40 * Product_{i=2..n} (81^i - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t b, uint64_t e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t prod = 40;          /* (9^2 - 1) / 2 */
    seq_t p = 81;             /* 81^i */
    idx_t i;
    for (i = 2; i <= n; i++) {
        p *= 81;
        prod *= (p - 1);
    }
    return pow_seq((seq_t)9, (uint64_t)n * (uint64_t)n) * prod;
}
