/*
 * A000738  Boustrophedon transform (first version) of Fibonacci numbers
 *          0,1,1,2,3,...
 * https://oeis.org/A000738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon triangle T(i,0) = F(i), T(i,k) = T(i,k-1) +
 *            T(i-1,i-k); a(n) = T(n,n), F = Fibonacci (F(0)=0, F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A738_MAX 10010

static seq_t a738_row[2][A738_MAX];
static seq_t a738_vals[A738_MAX];
static seq_t a738_fa, a738_fb;
static idx_t a738_done = 0;
static int a738_cur = 0;

seq_t a(idx_t n) {
    if (n >= A738_MAX) return 0;
    if (a738_done == 0) {
        a738_row[0][0] = 0;
        a738_vals[0] = 0;
        a738_fa = 0;
        a738_fb = 1;
        a738_cur = 0;
        a738_done = 1;
    }
    while (a738_done <= n) {
        idx_t i = a738_done;
        seq_t *p = a738_row[a738_cur];
        seq_t *c = a738_row[a738_cur ^ 1];
        c[0] = a738_fb;
        for (idx_t k = 1; k <= i; k++) {
            c[k] = c[k - 1] + p[i - k];
        }
        a738_vals[i] = c[i];
        seq_t t = a738_fa + a738_fb;
        a738_fa = a738_fb;
        a738_fb = t;
        a738_cur ^= 1;
        a738_done++;
    }
    return a738_vals[n];
}
