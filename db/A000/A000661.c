/*
 * A000661  Shifts 2 places left under boustrophedon transform.
 * https://oeis.org/A000661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(0) = 1, a(1) = 0, a(n+2) = T(n,n), where T is the
 *            boustrophedon (Seidel-Entringer) triangle of a: T(m,0) = a(m),
 *            T(m,k) = T(m,k-1) + T(m-1,m-k) for 1 <= k <= m. Equivalently
 *            a(n+2) = Sum_{k=0..n} binomial(n,k)*a(k)*A000111(n-k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 25, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A661_MAX 10010

static seq_t a661_av[A661_MAX + 3];
static seq_t a661_r0[A661_MAX + 3];
static seq_t a661_r1[A661_MAX + 3];
static seq_t *a661_prev = a661_r0;
static seq_t *a661_cur = a661_r1;
static idx_t a661_alen = 0;   /* number of known terms a(0..alen-1) */
static idx_t a661_row = 0;    /* index of the row stored in a661_prev */

seq_t a(idx_t n) {
    if (a661_alen == 0) {
        a661_av[0] = 1;
        a661_av[1] = 0;
        a661_prev[0] = a661_av[0];   /* row 0 */
        a661_av[2] = a661_prev[0];
        a661_row = 0;
        a661_alen = 3;
    }
    while (a661_alen <= n && a661_row + 1 <= A661_MAX) {
        idx_t m = a661_row + 1;
        idx_t k;
        seq_t *tmp;
        a661_cur[0] = a661_av[m];
        for (k = 1; k <= m; k++)
            a661_cur[k] = a661_cur[k - 1] + a661_prev[m - k];
        a661_av[m + 2] = a661_cur[m];
        tmp = a661_prev;
        a661_prev = a661_cur;
        a661_cur = tmp;
        a661_row = m;
        a661_alen = m + 3;
    }
    if (n >= a661_alen) return 0;
    return a661_av[n];
}
