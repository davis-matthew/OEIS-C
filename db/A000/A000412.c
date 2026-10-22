/*
 * A000412  Number of bipartite partitions of n white objects and 3 black
 *          ones.
 * https://oeis.org/A000412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A000041(k) * (A001399(n-k) + (n-k+1) + 1),
 *            from G.f. P(x) * (1/((1-x)(1-x^2)(1-x^3)) + 1/(1-x)^2 +
 *            1/(1-x)), where P(x) = Product_{i>=1} 1/(1-x^i) and A001399(m) =
 *            round((m+3)^2/12); A000041 is computed by Euler's pentagonal
 *            recurrence
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 83, u64 n <= 313,
 *            u128 n <= 1213, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"

static seq_t *Ptab = 0;
static idx_t Pcnt = 0;   /* number of p(k) computed */
static idx_t Pcap = 0;

static void ensure_p(idx_t n) {
    if (n < Pcnt) return;
    if (n + 1 > Pcap) {
        idx_t nc = Pcap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        seq_t *nt = alloc_zero(nc);
        for (idx_t i = 0; i < Pcnt; i++) nt[i] = Ptab[i];
        Ptab = nt;
        Pcap = nc;
    }
    for (idx_t m = Pcnt; m <= n; m++) {
        if (m == 0) { Ptab[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += Ptab[m - g1];
                if (g2 <= m) pos += Ptab[m - g2];
            } else {
                neg += Ptab[m - g1];
                if (g2 <= m) neg += Ptab[m - g2];
            }
        }
        Ptab[m] = pos - neg;
    }
    Pcnt = n + 1;
}

seq_t a(idx_t n) {
    ensure_p(n);
    seq_t s = 0;
    for (idx_t k = 0; k <= n; k++) {
        idx_t m = n - k;
        seq_t t = (seq_t)m + 3;
        seq_t parts3 = (t * t + 6) / 12;
        seq_t S = parts3 + (seq_t)m + 2;
        s += Ptab[k] * S;
    }
    return s;
}
