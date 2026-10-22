/*
 * A000702  a(n) is the number of conjugacy classes in the alternating group
 *          A_n.
 * https://oeis.org/A000702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (p(n) + 3Q(n))/2 for n>1, computed as b(n) with b(0) = 2
 *            and Sum_{k>=0} (-1)^k*b(n-(3k^2 +- k)/2) = 3(-1)^t if n = 2t^2
 *            (t>=1), else 0, for n >= 1 (from the generating function (P(x) +
 *            3Q(x))/2); a(1) = 1, a(n) = b(n) for n>1
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 134, u64 n <= 428,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A702_MAX 30001

static seq_t a702_memo[A702_MAX];
static idx_t a702_done = 0;

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    if (n >= A702_MAX) return 0;
    if (a702_done == 0) {
        a702_memo[0] = 2;
        a702_done = 1;
    }
    while (a702_done <= n) {
        idx_t m = a702_done;
        seq_t pos = 0, neg = 0;
        idx_t k, t;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) pos += a702_memo[m - g1];
            else neg += a702_memo[m - g1];
            if (g2 <= m) {
                if (k & 1) pos += a702_memo[m - g2];
                else neg += a702_memo[m - g2];
            }
        }
        if (m % 2 == 0) {
            idx_t h = m / 2;
            t = 1;
            while (t * t < h) t++;
            if (t * t == h) {
                if (t & 1) neg += 3;
                else pos += 3;
            }
        }
        a702_memo[m] = pos - neg;
        a702_done++;
    }
    return a702_memo[n];
}
