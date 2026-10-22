/*
 * A007044  Left diagonal of partition triangle A047812.
 * https://oeis.org/A007044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(2n+2) - 2*Sum_{i=0..n+1} p(i) + 3, where p = A000041
 *            (partition numbers); this is the coefficient of q^(2n+2) in the
 *            Gaussian binomial [2n, n]_q, i.e. the number of partitions of
 *            2n+2 into at most n parts, each part at most n
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 62, u64 n <= 207, u128 n <= 500,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *P = NULL;
static idx_t Plen = 0;

static void ensure_p(idx_t m) {
    idx_t k;
    if (m < Plen) return;
    P = grow_zero(P, Plen, m + 1);
    for (k = Plen; k <= m; k++) {
        if (k == 0) { P[0] = 1; continue; }
        {
            seq_t pos = 0, neg = 0;
            idx_t j;
            for (j = 1; ; j++) {
                idx_t g1 = j * (3 * j - 1) / 2;
                idx_t g2 = j * (3 * j + 1) / 2;
                bool plus = (j & 1) != 0;
                if (g1 > k) break;
                if (plus) pos += P[k - g1]; else neg += P[k - g1];
                if (g2 <= k) {
                    if (plus) pos += P[k - g2]; else neg += P[k - g2];
                }
            }
            P[k] = pos - neg;
        }
    }
    Plen = m + 1;
}

seq_t a(idx_t n) {
    idx_t m = 2 * n + 2, i;
    seq_t s = 0, r;
    ensure_p(m);
    for (i = 0; i <= n + 1; i++) s += P[i];
    r = P[m] + 3;
    r -= s;
    r -= s;
    return r;
}
