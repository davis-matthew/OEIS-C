/*
 * A002067  a(n) = Sum_{k=0..n-1} binomial(2*n,2*k)*a(k)*a(n-k-1).
 * https://oeis.org/A002067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen, easy, nice
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(2*n,2*k)*a(k)*a(n-k-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A2067_MAXN 10001

static seq_t a2067_val[A2067_MAXN + 1];
static seq_t a2067_row[2 * A2067_MAXN + 3];
static idx_t a2067_have = 0;   /* number of terms computed */
static idx_t a2067_r = 0;      /* current Pascal row index held in a2067_row */

static void a2067_step_row(void) {
    idx_t r = a2067_r + 1;
    a2067_row[r] = 1;
    for (idx_t j = r - 1; j >= 1; j--) {
        a2067_row[j] += a2067_row[j - 1];
    }
    a2067_r = r;
}

seq_t a(idx_t n) {
    if (n > A2067_MAXN) return 0;
    if (a2067_have == 0) {
        a2067_val[0] = 1;
        a2067_row[0] = 1;
        a2067_r = 0;
        a2067_have = 1;
    }
    while (a2067_have <= n) {
        idx_t m = a2067_have;
        while (a2067_r < 2 * m) a2067_step_row();
        seq_t s = 0;
        for (idx_t k = 0; k < m; k++) {
            seq_t t = a2067_row[2 * k] * a2067_val[k];
            t *= a2067_val[m - k - 1];
            s += t;
        }
        a2067_val[m] = s;
        a2067_have = m + 1;
    }
    return a2067_val[n];
}
