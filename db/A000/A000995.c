/*
 * A000995  Shifts left two terms under the binomial transform.
 * https://oeis.org/A000995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen, easy, nice
 * Formula:   a(0) = 0, a(1) = 1, a(n) = Sum_{k=0..n-2} binomial(n-2,k)*a(k)
 *            for n >= 2 (from A(x) = x*(1 + x*A(x/(1-x))/(1-x))), computed
 *            with a running difference triangle D(m,i) = D(m-1,i-1) +
 *            D(m,i-1), D(m,0) = a(m), a(m+2) = D(m,m)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 19, u64 n <= 31, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A995_CAP 12002

static seq_t a995_vals[A995_CAP];
static seq_t a995_row[A995_CAP];
static idx_t a995_done = 0;   /* rows 0..a995_done-1 of the triangle computed */
static int a995_init = 0;

seq_t a(idx_t n) {
    if (!a995_init) {
        a995_vals[0] = 0;
        a995_vals[1] = 1;
        a995_init = 1;
    }
    if (n < 2) return a995_vals[n];
    if (n >= A995_CAP) return 0;
    while (a995_done + 2 <= n) {
        idx_t m = a995_done;
        seq_t prev = a995_row[0];
        a995_row[0] = a995_vals[m];
        for (idx_t i = 1; i <= m; i++) {
            seq_t old = a995_row[i];
            a995_row[i] = prev + a995_row[i - 1];
            prev = old;
        }
        a995_vals[m + 2] = a995_row[m];
        a995_done++;
    }
    return a995_vals[n];
}
