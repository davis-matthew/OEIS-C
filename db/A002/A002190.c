/*
 * A002190  Sum_{n>=0} a(n)*x^n/n!^2 = -log(BesselJ(0,2*sqrt(x))).
 * https://oeis.org/A002190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 0, a(1) = 1; a(n) = (-1)^(n+1) + Sum_{k=1..n-1}
 *            (-1)^(n-k+1) * binomial(n,k) * binomial(n-1,k-1) * a(k) (from
 *            J*(x f') = -x J' with f = -log J, J = Sum (-x)^n/n!^2, written
 *            in the basis x^n/(n!(n-1)!); no division needed)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A_memo = NULL;   /* A_memo[k] = a(k) for 1 <= k <= done */
static seq_t *rowP = NULL;     /* Pascal row `done` */
static seq_t *rowQ = NULL;     /* scratch for next Pascal row */
static idx_t cap = 0;
static idx_t done = 0;

static void ensure_cap(idx_t need) {
    if (A_memo == NULL) {
        idx_t c = 16;
        while (c < need) c *= 2;
        A_memo = alloc_zero(c);
        rowP = alloc_zero(c);
        rowQ = alloc_zero(c);
        cap = c;
        rowP[0] = 1;           /* row 0 */
        done = 0;
        return;
    }
    if (need <= cap) return;
    idx_t c = cap * 2;
    while (c < need) c *= 2;
    A_memo = grow_zero(A_memo, cap, c);
    rowP = grow_zero(rowP, cap, c);
    rowQ = grow_zero(rowQ, cap, c);
    cap = c;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_cap(n + 2);
    while (done < n) {
        idx_t m = done + 1;
        /* rowQ = Pascal row m from rowP = row m-1 */
        rowQ[0] = 1;
        for (idx_t k = 1; k <= m; k++) {
            seq_t hi = (k <= m - 1) ? rowP[k] : (seq_t)0;
            rowQ[k] = rowP[k - 1] + hi;
        }
        seq_t pos = 0, neg = 0;
        if (m & 1) pos = pos + 1; else neg = neg + 1;
        for (idx_t k = 1; k < m; k++) {
            seq_t term = rowQ[k] * rowP[k - 1] * A_memo[k];
            if ((m - k) & 1) pos = pos + term;
            else neg = neg + term;
        }
        A_memo[m] = pos - neg;
        seq_t *t = rowP; rowP = rowQ; rowQ = t;
        done = m;
    }
    return A_memo[n];
}
