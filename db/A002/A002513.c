/*
 * A002513  Number of "cubic partitions" of n: expansion of Product_{k>0}
 *          1/((1-x^(2k))^2*(1-x^(2k-1))) in powers of x.
 * https://oeis.org/A002513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} p(j)*p(n-2*j), where p = A000041
 *            (partition numbers), since G.f. = Product_{k>0}
 *            1/((1-x^k)*(1-x^(2k))) = P(x)*P(x^2); p(m) = Sum_{k>=1}
 *            (-1)^(k+1) * (p(m-k*(3k-1)/2) + p(m-k*(3k+1)/2)), p(0) = 1
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 91, u64 n <= 290,
 *            u128 n <= 1002, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *a002513_P = NULL;
static idx_t a002513_cap = 0;

static void a002513_ensure(idx_t n) {
    if (n < a002513_cap) return;
    idx_t nc = a002513_cap * 2;
    if (nc < n + 1) nc = n + 1;
    seq_t *q = alloc_zero(nc);
    for (idx_t i = 0; i < a002513_cap; i++) q[i] = a002513_P[i];
    for (idx_t m = a002513_cap; m < nc; m++) {
        if (m == 0) { q[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t term = q[m - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) term += q[m - g2];
            if (k & 1) pos += term; else neg += term;
        }
        q[m] = pos - neg;
    }
    a002513_P = q;
    a002513_cap = nc;
}

seq_t a(idx_t n) {
    a002513_ensure(n);
    seq_t s = 0;
    for (idx_t j = 0; 2 * j <= n; j++)
        s += a002513_P[j] * a002513_P[n - 2 * j];
    return s;
}
