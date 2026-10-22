/*
 * A005739  Number of disjunctively-realizable functions of n variables.
 * https://oeis.org/A005739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*T(k), where T(k) = A005616(k)
 *            (binomial transform); T(0)=2, T(1)=2, and for m>=2 with R1(m) =
 *            Sum_{k=1..m-1} binomial(m-1,k-1)*u(k)*e1(m-k), R2(m) =
 *            Sum_{k=1..m-1} binomial(m-1,k-1)*v(k)*e2(m-k): u(m) = 2*R2+R1,
 *            v(m) = R1, e1(m) = u(m)+R1, e2(m) = v(m)+R2, T(m) =...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *su = 0, *sv = 0, *se1 = 0, *se2 = 0, *sT = 0, *srow = 0, *sa = 0;
static idx_t cnt = 0, cap = 0;

static void ensure(idx_t n)
{
    idx_t m, k, j;
    if (cnt != 0 && n < cnt) return;
    if (n + 2 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 2) nc = n + 2;
        if (nc < 16) nc = 16;
        su = grow_zero(su, cap, nc);
        sv = grow_zero(sv, cap, nc);
        se1 = grow_zero(se1, cap, nc);
        se2 = grow_zero(se2, cap, nc);
        sT = grow_zero(sT, cap, nc);
        srow = grow_zero(srow, cap, nc);
        sa = grow_zero(sa, cap, nc);
        cap = nc;
    }
    if (cnt == 0) {
        su[0] = 0; sv[0] = 0; se1[0] = 1; se2[0] = 1; sT[0] = 2; sa[0] = 2;
        su[1] = 2; sv[1] = 1; se1[1] = 2; se2[1] = 1; sT[1] = 2; sa[1] = 4;
        srow[0] = 1; srow[1] = 1; /* binomial(1, .) */
        cnt = 2;
    }
    for (m = cnt; m <= n; m++) {
        /* srow holds binomial(m-1, .) */
        seq_t R1 = 0, R2 = 0, s;
        for (k = 1; k < m; k++) {
            R1 += srow[k - 1] * su[k] * se1[m - k];
            R2 += srow[k - 1] * sv[k] * se2[m - k];
        }
        su[m] = 2 * R2 + R1;
        sv[m] = R1;
        se1[m] = su[m] + R1;
        se2[m] = sv[m] + R2;
        sT[m] = 2 * (R1 + R2);
        /* advance to binomial(m, .) */
        srow[m] = 1;
        for (j = m - 1; j >= 1; j--) srow[j] += srow[j - 1];
        s = 0;
        for (k = 0; k <= m; k++) s += srow[k] * sT[k];
        sa[m] = s;
        cnt = m + 1;
    }
}

seq_t a(idx_t n)
{
    ensure(n);
    return sa[n];
}
