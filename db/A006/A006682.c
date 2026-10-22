/*
 * A006682  Number of binary phylogenetic trees with n labels.
 * https://oeis.org/A006682
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} S(n,k)*A006680(k), with A006680(k) the
 *            number of unrooted binary trees (every node of degree 1 or 3) on
 *            k labels, all leaves labeled and internal nodes optionally
 *            labeled. Equivalently each node carries a set of labels
 *            (nonempty at the leaves), and with E = e^x - 1: R = E + e^...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *R_ = 0, *P_ = 0, *T_ = 0, *ROW = 0, *AV = 0;
static idx_t cap = 0, done = 0;

static void ensure(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    if (cap == 0) {
        R_ = alloc_zero(nc);
        P_ = alloc_zero(nc);
        T_ = alloc_zero(nc);
        ROW = alloc_zero(nc);
        AV = alloc_zero(nc);
        ROW[0] = 1;
    } else {
        R_ = grow_zero(R_, cap, nc);
        P_ = grow_zero(P_, cap, nc);
        T_ = grow_zero(T_, cap, nc);
        ROW = grow_zero(ROW, cap, nc);
        AV = grow_zero(AV, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    ensure(n + 2);
    while (done < n - 1) {
        idx_t m = done + 1, i;
        seq_t sp = 0, st = 0, sr = 0, sa = 0;
        /* ROW holds row m-1 of Pascal's triangle */
        for (i = 1; i < m; i++) {
            seq_t c = ROW[i - 1] * R_[i];
            sp += c * R_[m - i];
            st += c * P_[m - i];
        }
        P_[m] = sp;
        T_[m] = st;
        /* advance ROW to row m */
        for (i = m; i >= 1; i--) ROW[i] += ROW[i - 1];
        for (i = 0; i <= m; i++) sr += ROW[i] * P_[i];
        R_[m] = sr + 1;
        for (i = 0; i <= m; i++) sa += ROW[i] * (R_[i] + T_[i]);
        AV[m + 1] = sa;
        done = m;
    }
    return AV[n];
}
