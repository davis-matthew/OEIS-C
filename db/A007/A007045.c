/*
 * A007045  Second (lower) diagonal of partition triangle A047812.
 * https://oeis.org/A007045
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of partitions of 2n+3 into at most n parts each <=
 *            n (coefficient of q^(2n+3) in the Gaussian binomial [2n,n]_q) =
 *            p(2n+3) - 2*Sum_{r=0..n+2} p(r) + 9, where p = A000041 is
 *            computed by Euler's pentagonal recurrence p(i) = Sum_{k>=1}
 *            (-1)^(k+1) (p(i-k(3k-1)/2) + p(i-k(3k+1)/2)), p(0)...
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 62, u64 n <= 206, u128 n <= 500,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *P = NULL;
static idx_t plen = 0;

static void ensure_p(idx_t m) {
    idx_t nl, i, k;
    if (m < plen) return;
    nl = m + 1;
    if (nl < 2 * plen) nl = 2 * plen;
    if (P == NULL) P = alloc_zero(nl);
    else P = grow_zero(P, plen, nl);
    for (i = plen; i < nl; i++) {
        seq_t pos = 0, neg = 0;
        if (i == 0) { P[0] = 1; continue; }
        for (k = 1; ; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                pos += P[i - g1];
                if (g2 <= i) pos += P[i - g2];
            } else {
                neg += P[i - g1];
                if (g2 <= i) neg += P[i - g2];
            }
        }
        P[i] = pos - neg;
    }
    plen = nl;
}

seq_t a(idx_t n) {
    seq_t s = 0, res;
    idx_t r;
    if (n < 2) return 0;
    ensure_p(2 * n + 3);
    for (r = 0; r <= n + 2; r++) s += P[r];
    res = P[2 * n + 3] + 9;
    res -= s;
    res -= s;
    return res;
}
