/*
 * A005348  Number of ways to add n ordinals.
 * https://oeis.org/A005348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max over partitions n-1 = k_1+...+k_r of Product_i
 *            (k_i*2^(k_i-1) + 1), a(1) = 1 (so a(n) = (n-1)*2^(n-2) + 1 for n
 *            <= 8), and a(n) = 81*a(n-5) for n >= 21
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   a005348_h
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005348_h.h"

seq_t a(idx_t n) {
    seq_t b[32];
    seq_t r;
    idx_t m, q, i, k;

    if (n <= 1) return (seq_t)1;
    m = n - 1;
    q = 0;
    if (m > 24) {
        q = (m - 20) / 5;
        m -= 5 * q;
    }

    b[0] = (seq_t)1;
    for (i = 1; i <= m; i++) {
        seq_t best = (seq_t)0;
        for (k = 1; k <= i; k++) {
            seq_t c = a005348_h(k) * b[i - k];
            if (c > best) best = c;
        }
        b[i] = best;
    }

    r = b[m];
    for (i = 0; i < q; i++) r = r * 81;
    return r;
}
