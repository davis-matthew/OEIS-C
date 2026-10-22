/*
 * A006128  Total number of parts in all partitions of n. Also, sum of largest
 *          parts of all partitions of n.
 * https://oeis.org/A006128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{m=1..n} A000005(m)*A000041(n-m), with p(n) =
 *            A000041(n) from Euler's pentagonal recurrence
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 100, u64 n <= 354,
 *            u128 n <= 1318, big any n
 * Helpers:   alloc_zero, divisor_count, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/divisor_count.h"

static seq_t *P = 0;
static idx_t Plen = 0;

static void ensure_p(idx_t n) {
    if (n < Plen) return;
    idx_t newlen = n + 1;
    if (newlen < 2 * Plen) newlen = 2 * Plen;
    seq_t *q = alloc_zero(newlen);
    for (idx_t i = 0; i < Plen; i++) q[i] = P[i];
    if (P) release(P, Plen);
    P = q;
    idx_t start = Plen;
    Plen = newlen;
    for (idx_t i = start; i < Plen; i++) {
        if (i == 0) { P[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            idx_t g2 = k * (3 * k + 1) / 2;
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
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_p(n);
    seq_t result = 0;
    for (idx_t m = 1; m <= n; m++) {
        result += (seq_t)divisor_count(m) * P[n - m];
    }
    return result;
}
