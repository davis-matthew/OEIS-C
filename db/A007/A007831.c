/*
 * A007831  Number of edge-labeled series-reduced trees with n nodes.
 * https://oeis.org/A007831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = Sum_{k=0..n-2} (-1)^k * C(n,k) *
 *            C(n-1,k) * k! * (n+1-k)^(n-2-k) + (-1)^(n-1) * n!/2, which
 *            equals A005512(n+1)/(n+1), where A005512(m) = Sum_{k=0..m-2}
 *            (-1)^k * C(m,k) * C(m-2,k) * k! * (m-k)^(m-2-k)
 *            (inclusion-exclusion over vertices of degree 2, using Pruefer
 *            codes)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   a000272_ipow, binom, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/a000272_ipow.h"
#include "../utils/seq_is_big.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    bool big = seq_is_big() ? true : false;
    seq_t pos = 0, neg = 0;
    seq_t f = 1;      /* n!/(n-k)! */
    seq_t c = 1;      /* C(n-1,k), maintained incrementally in the big build */
    seq_t half_fact = 0;
    for (idx_t k = 0; k + 2 <= n; k++) {
        seq_t cb = big ? c : binom(n - 1, k);
        seq_t term = f * cb;
        term = term * a000272_ipow((seq_t)(n + 1 - k), (idx_t)(n - 2 - k));
        if (k % 2 == 0) pos += term; else neg += term;
        if (k + 2 == n) half_fact = f;   /* n!/2 */
        f = f * (seq_t)(n - k);
        if (big) c = c * (seq_t)(n - 1 - k) / (seq_t)(k + 1);
    }
    if ((n - 1) % 2 == 0) pos += half_fact; else neg += half_fact;
    return pos - neg;
}
