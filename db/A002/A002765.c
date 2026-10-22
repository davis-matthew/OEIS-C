/*
 * A002765  Number of bipartite partitions.
 * https://oeis.org/A002765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} S(k)*p(n-k), where p = A000041 (partitions)
 *            and S(k) = 1 + 3(k+1) + (k+1)(k+2)/2 + Sum_{i<=k} h2(i) + h3(k)
 *            + Sum_{i<=k} h3(i), h2(i) = floor(i/2)+1, h3(i) =
 *            round((i+3)^2/12). Computed as v(m) = S(m) + Sum_{j>=1}
 *            (-1)^(j+1) [v(m - j(3j-1)/2) + v(m - j(3j+1)/2)] (Euler's
 *            penta...
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define A2765_MAX 100001

static seq_t a2765_v[A2765_MAX];
static idx_t a2765_done = 0;
static seq_t a2765_ah2;
static seq_t a2765_ah3;

seq_t a(idx_t n) {
    idx_t m;
    if (n >= A2765_MAX) n = A2765_MAX - 1;
    for (m = a2765_done; m <= n; m++) {
        seq_t K = (seq_t)m;
        seq_t h2 = K / 2 + 1;
        seq_t h3 = ((K + 3) * (K + 3) + 6) / 12;
        seq_t S;
        seq_t pos;
        seq_t neg = 0;
        idx_t j;
        a2765_ah2 += h2;
        a2765_ah3 += h3;
        S = (seq_t)1 + (seq_t)3 * (K + 1) + (K + 1) * (K + 2) / 2 + a2765_ah2 + h3 + a2765_ah3;
        pos = S;
        for (j = 1;; j++) {
            idx_t g1 = j * (3 * j - 1) / 2;
            idx_t g2 = j * (3 * j + 1) / 2;
            if (g1 > m) break;
            if (j & 1) {
                pos += a2765_v[m - g1];
                if (g2 <= m) pos += a2765_v[m - g2];
            } else {
                neg += a2765_v[m - g1];
                if (g2 <= m) neg += a2765_v[m - g2];
            }
        }
        a2765_v[m] = pos - neg;
        a2765_done = m + 1;
    }
    return a2765_v[n];
}
