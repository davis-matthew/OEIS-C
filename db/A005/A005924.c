/*
 * A005924  From solution to a difference equation.
 * https://oeis.org/A005924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A000557(n) - 1, where A000557 has e.g.f. 1/(1 -
 *            2*sinh(x)), so b(0) = 1, b(m) = 2 * Sum_{k odd, 1<=k<=m}
 *            binomial(m,k) * b(m-k), and a(n) = b(n) - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define A5924_MAX 20002

static seq_t a5924_b[A5924_MAX];
static idx_t a5924_cnt = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a5924_cnt == 0) {
        a5924_b[0] = 1;
        a5924_cnt = 1;
    }
    while (a5924_cnt <= n && a5924_cnt < A5924_MAX) {
        idx_t m = a5924_cnt;
        seq_t s = 0;
        seq_t c = 1; /* binomial(m, 0) */
        idx_t k;
        for (k = 1; k <= m; k++) {
            c = c * (seq_t)(m - k + 1) / (seq_t)k;
            if (k & 1) s += c * a5924_b[m - k];
        }
        a5924_b[m] = s * 2;
        a5924_cnt++;
    }
    return a5924_b[n] - 1;
}
