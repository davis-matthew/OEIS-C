/*
 * A001256  Squares of numbers of trees.
 * https://oeis.org/A001256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000055(n)^2, where A000055(n) = r(n) - (Sum_{k=1..n-1}
 *            r(k)*r(n-k) - [n even]*r(n/2))/2, a(0) = 1, and r = A000081
 *            (rooted trees): r(0) = 0, r(1) = 1, r(m+1) = (1/m) *
 *            Sum_{k=1..m} s(k)*r(m-k+1) with s(k) = Sum_{d|k} d*r(d)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAXN 10002

static seq_t r[MAXN];
static seq_t s[MAXN];
static idx_t done = 0;

static void add_s(idx_t m) {
    seq_t v = (seq_t)m * r[m];
    for (idx_t k = m; k < MAXN; k += m)
        s[k] += v;
}

static void ensure(idx_t n) {
    if (done == 0) {
        r[0] = 0;
        r[1] = 1;
        add_s(1);
        done = 1;
    }
    while (done < n) {
        idx_t m = done;
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; k++)
            sum += s[k] * r[m - k + 1];
        r[m + 1] = sum / (seq_t)m;
        add_s(m + 1);
        done = m + 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n >= MAXN) return 0;
    ensure(n);
    seq_t S = 0;
    for (idx_t k = 1; k < n; k++)
        S += r[k] * r[n - k];
    if (n % 2 == 0)
        S -= r[n / 2];
    seq_t t = r[n] - S / 2;
    return t * t;
}
