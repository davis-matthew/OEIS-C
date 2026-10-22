/*
 * A000831  Expansion of e.g.f. (1 + tan(x))/(1 - tan(x)).
 * https://oeis.org/A000831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * A000111(n), where A000111(n) = E(n,n) from the
 *            Entringer triangle: E(0,0) = 1, E(n,0) = 0 for n > 0, E(n,k) =
 *            E(n,k-1) + E(n-1,n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A831_MAXN 20000

static seq_t a831_euler[A831_MAXN + 1];
static seq_t a831_rowA[A831_MAXN + 2];
static seq_t a831_rowB[A831_MAXN + 2];
static seq_t *a831_prev = a831_rowA;
static seq_t *a831_cur = a831_rowB;
static idx_t a831_done = 0;
static int a831_init = 0;

static void a831_extend(idx_t n) {
    if (!a831_init) {
        a831_prev[0] = (seq_t)1;
        a831_euler[0] = (seq_t)1;
        a831_done = 0;
        a831_init = 1;
    }
    while (a831_done < n) {
        idx_t m = a831_done + 1;
        seq_t *p = a831_prev;
        seq_t *c = a831_cur;
        c[0] = (seq_t)0;
        for (idx_t k = 1; k <= m; k++) {
            c[k] = c[k - 1] + p[m - k];
        }
        a831_euler[m] = c[m];
        a831_prev = c;
        a831_cur = p;
        a831_done = m;
    }
}

seq_t a(idx_t n) {
    if (n > A831_MAXN) return (seq_t)0;
    a831_extend(n);
    seq_t r = a831_euler[n];
    for (idx_t i = 0; i < n; i++) {
        r = r + r;
    }
    return r;
}
