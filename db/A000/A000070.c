/*
 * A000070  a(n) = Sum_{k=0..n} p(k) where p(k) = number of partitions of k
 *          (A000041).
 * https://oeis.org/A000070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} p(k), where p(0) = 1 and p(m) = Sum_{k>=1}
 *            (-1)^(k+1) * (p(m - k*(3k-1)/2) + p(m - k*(3k+1)/2)) (Euler's
 *            pentagonal theorem)
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 107, u64 n <= 372,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *P_tab = 0;
static seq_t *S_tab = 0;
static idx_t have = 0;
static idx_t cap = 0;

static void ensure(idx_t n)
{
    idx_t m, i;
    if (n + 1 > cap) {
        idx_t newcap = cap * 2;
        seq_t *NP, *NS;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 64) newcap = 64;
        NP = alloc_series((size_t)newcap);
        NS = alloc_series((size_t)newcap);
        for (i = 0; i < have; i++) {
            NP[i] = P_tab[i];
            NS[i] = S_tab[i];
        }
        P_tab = NP;
        S_tab = NS;
        cap = newcap;
    }
    for (m = have; m <= n; m++) {
        if (m == 0) {
            P_tab[0] = 1;
            S_tab[0] = 1;
        } else {
            seq_t pos = 0, neg = 0;
            idx_t k;
            for (k = 1;; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                idx_t g2 = k * (3 * k + 1) / 2;
                if (g1 > m) break;
                if (k & 1) {
                    pos += P_tab[m - g1];
                    if (g2 <= m) pos += P_tab[m - g2];
                } else {
                    neg += P_tab[m - g1];
                    if (g2 <= m) neg += P_tab[m - g2];
                }
            }
            P_tab[m] = pos - neg;
            S_tab[m] = S_tab[m - 1] + P_tab[m];
        }
    }
    if (n + 1 > have) have = n + 1;
}

seq_t a(idx_t n)
{
    if (n >= have) ensure(n);
    return S_tab[n];
}
