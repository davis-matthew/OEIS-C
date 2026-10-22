/*
 * A000551  Number of labeled rooted trees of height 2 with n nodes.
 * https://oeis.org/A000551
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(Sum_{k=1..n-1} binomial(n-1,k)*k^(n-1-k) - 1), from
 *            E.g.f. x*(exp(x*exp(x))-exp(x))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   a551_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a551_pw.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t m = n - 1;
    seq_t s = 0;
    seq_t c = 1; /* binomial(m, k) */
    idx_t k;
    for (k = 1; k <= m; k++) {
        c = c * (seq_t)(m - k + 1) / (seq_t)k;
        s += c * a551_pw((seq_t)k, m - k);
    }
    return (seq_t)n * (s - 1);
}
