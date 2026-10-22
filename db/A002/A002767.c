/*
 * A002767  Number of bipartite partitions.
 * https://oeis.org/A002767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} p(n-k)*c(k), where p = A000041 (partition
 *            numbers) and c(k) = (k+2) + (floor(3k/2)+2) + round((k+3)^2/12),
 *            i.e. G.f.: (1/(1-x) + 1/(1-x)^2 + (2+x)/((1-x)(1-x^2)) +
 *            1/((1-x)(1-x^2)(1-x^3))) / Prod_{j>=1}(1-x^j). p(m) is computed
 *            with Euler's pentagonal-number recurrence, p(0) = 1
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A2767_MAX 100001

static seq_t a2767_p[A2767_MAX];
static idx_t a2767_have = 0;

static void a2767_fill(idx_t n)
{
    idx_t m;
    if (a2767_have == 0) {
        a2767_p[0] = 1;
        a2767_have = 1;
    }
    for (m = a2767_have; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        idx_t k;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2;
            if (g1 > m) break;
            g2 = g1 + k;
            if (k & 1) {
                pos += a2767_p[m - g1];
                if (g2 <= m) pos += a2767_p[m - g2];
            } else {
                neg += a2767_p[m - g1];
                if (g2 <= m) neg += a2767_p[m - g2];
            }
        }
        a2767_p[m] = pos - neg;
    }
    if (n + 1 > a2767_have) a2767_have = n + 1;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    if (n >= A2767_MAX) return 0;
    a2767_fill(n);
    for (k = 0; k <= n; k++) {
        idx_t t = k + 3;
        idx_t c = (k + 2) + (3 * k) / 2 + 2 + (t * t + 6) / 12;
        s += a2767_p[n - k] * (seq_t)c;
    }
    return s;
}
