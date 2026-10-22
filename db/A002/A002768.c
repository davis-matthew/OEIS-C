/*
 * A002768  Number of bipartite partitions.
 * https://oeis.org/A002768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A000041(k)*b(n-k), where b has g.f. (5 - 2x
 *            + x^2 + x^6(1-x)/((1-x^2)(1-x^3))) / (1-x)^2, i.e. b(n) = b(n-1)
 *            + d(n), d(n) = d(n-1) + e(n), e = 5,-2,1,0,0,0, then period
 *            (1,-1,1,0,0,0). Computed as a(n) = b(n) + Sum_{k>=1} (-1)^(k+1)
 *            (a(n-k(3k-1)/2) + a(n-k(3k+1)/2))
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   a002768_e
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a002768_e.h"

static seq_t *A_cache = NULL;
static long long *B_cache = NULL;
static idx_t A_len = 0;

seq_t a(idx_t n) {
    if (n >= A_len) {
        idx_t newlen = n + 1;
        if (newlen < 2 * A_len) newlen = 2 * A_len;
        if (newlen < 64) newlen = 64;
        seq_t *na = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
        long long *nb = (long long *)malloc((size_t)newlen * sizeof(long long));
        long long d = 0, b = 0;
        for (idx_t i = 0; i < newlen; i++) {
            d += a002768_e(i);
            b += d;
            nb[i] = b;
        }
        for (idx_t i = 0; i < A_len; i++) na[i] = A_cache[i];
        for (idx_t i = A_len; i < newlen; i++) {
            seq_t pos = (seq_t)nb[i];
            seq_t neg = 0;
            for (idx_t k = 1;; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                idx_t g2 = k * (3 * k + 1) / 2;
                if (k & 1) {
                    pos += na[i - g1];
                    if (g2 <= i) pos += na[i - g2];
                } else {
                    neg += na[i - g1];
                    if (g2 <= i) neg += na[i - g2];
                }
            }
            na[i] = pos - neg;
        }
        free(A_cache);
        free(B_cache);
        A_cache = na;
        B_cache = nb;
        A_len = newlen;
    }
    return A_cache[n];
}
