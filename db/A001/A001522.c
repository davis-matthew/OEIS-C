/*
 * A001522  Number of n-stacks with strictly receding walls, or the number of
 *          Type A partitions of n in the sense of Auluck (1951).
 * https://oeis.org/A001522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: 1 + ( Sum_{k>=1} -(-1)^k * x^(k*(k+1)/2) ) / (
 *            Product_{k>=1} 1-x^k ), i.e. a(0) = 1, a(n) = Sum_{k>=1,
 *            k*(k+1)/2<=n} (-1)^(k+1) * A000041(n - k*(k+1)/2), with A000041
 *            from Euler's pentagonal recurrence
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 134, u64 n <= 428,
 *            u128 n <= 1479, big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *P = NULL;   /* partition numbers p(0..pcount-1) */
static idx_t pcount = 0;
static idx_t pcap = 0;

static void ensure_p(idx_t n) {
    if (n < pcount) return;
    if (n + 1 > pcap) {
        idx_t nc = pcap ? pcap : 64;
        while (nc < n + 1) nc *= 2;
        P = grow_zero(P, pcount, nc);
        pcap = nc;
    }
    for (idx_t m = pcount; m <= n; m++) {
        if (m == 0) { P[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            int plus = (k & 1) != 0;
            if (plus) pos += P[m - g1]; else neg += P[m - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) {
                if (plus) pos += P[m - g2]; else neg += P[m - g2];
            }
        }
        P[m] = pos - neg;
    }
    pcount = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    ensure_p(n);
    seq_t pos = 0, neg = 0;
    for (idx_t k = 1;; k++) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;
        if (k & 1) pos += P[n - t]; else neg += P[n - t];
    }
    return pos - neg;
}
