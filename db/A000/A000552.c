/*
 * A000552  Number of labeled rooted trees of height 3 with n nodes.
 * https://oeis.org/A000552
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n * Sum_{j>=1, k>=1, r>=1, j+k+r=n-1} (n-1)!/(k!*j!*r!) *
 *            k^j * j^r, which is n! [x^n] x*(exp(x*exp(x*exp(x))) -
 *            exp(x*exp(x))): k roots, j nodes at depth 1, r >= 1 nodes at
 *            depth 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   a552_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a552_pow.h"

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t total = 0;
    seq_t cmj = 1; /* C(m, j) */
    idx_t j;
    for (j = 1; j + 2 <= m; j++) {
        cmj = cmj * (seq_t)(m - j + 1) / (seq_t)j;
        idx_t s = m - j;
        seq_t inner = 0;
        seq_t c = 1;   /* C(s, r) */
        seq_t jp = 1;  /* j^r */
        idx_t r;
        for (r = 1; r + 1 <= s; r++) {
            idx_t k = s - r;
            c = c * (seq_t)(s - r + 1) / (seq_t)r;
            jp *= (seq_t)j;
            inner += c * a552_pow((seq_t)k, j) * jp;
        }
        total += cmj * inner;
    }
    return (seq_t)n * total;
}
