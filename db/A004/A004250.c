/*
 * A004250  Number of partitions of n into 3 or more parts.
 * https://oeis.org/A004250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000041(n) - floor((n+2)/2), with A000041 computed by
 *            Euler's pentagonal number recurrence p(m) = Sum_{k>=1}
 *            (-1)^(k+1) (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0) = 1
 * Range:     u8 n <= 16, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *pt = NULL;
static idx_t plen = 0;

static void ensure_p(idx_t n) {
    if (n < plen) return;
    idx_t newlen = n + 1;
    if (newlen < 2 * plen) newlen = 2 * plen;
    pt = grow_zero(pt, plen, newlen);
    idx_t start = plen;
    if (start == 0) {
        pt[0] = 1;
        start = 1;
    }
    for (idx_t m = start; m < newlen; m++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += pt[m - g1];
                if (g2 <= m) pos += pt[m - g2];
            } else {
                neg += pt[m - g1];
                if (g2 <= m) neg += pt[m - g2];
            }
        }
        pt[m] = pos - neg;
    }
    plen = newlen;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_p(n);
    seq_t result = pt[n];
    result -= (seq_t)((n + 2) / 2);
    return result;
}
