/*
 * A000303  Number of permutations of [n] in which the longest increasing run
 *          has length 2.
 * https://oeis.org/A000303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049774(n) - 1, where A049774(n) counts permutations of
 *            [n] with no increasing run longer than 2 (the only permutation
 *            with all runs of length 1 is the decreasing one). A049774(n) is
 *            computed with a DP over the rank j of the last element and
 *            whether the last step was an ascent: U(m+1,j) =...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/zalloc_s.h"

static seq_t *res_ = 0, *U_ = 0, *D_ = 0, *T_ = 0;
static idx_t cap_ = 0;   /* allocated length of each array */
static idx_t m_ = 0;     /* rows computed so far: res_[1..m_] valid */

static void grow(idx_t need)
{
    idx_t nc, i;
    seq_t *nr, *nu, *nd, *nt;
    if (need <= cap_) return;
    nc = cap_ * 2;
    if (nc < need) nc = need;
    if (nc < 64) nc = 64;
    nr = zalloc_s(nc);
    nu = zalloc_s(nc);
    nd = zalloc_s(nc);
    nt = zalloc_s(nc);
    for (i = 0; i < cap_; i++) {
        nr[i] = res_[i];
        nu[i] = U_[i];
        nd[i] = D_[i];
    }
    res_ = nr; U_ = nu; D_ = nd; T_ = nt;
    cap_ = nc;
}

seq_t a(idx_t n)
{
    idx_t j, m;
    seq_t run, tmp, tot;
    if (n < 1) return (seq_t)0;
    grow(n + 2);
    if (m_ == 0) {
        U_[1] = (seq_t)0;
        D_[1] = (seq_t)1;
        res_[1] = (seq_t)1;
        m_ = 1;
    }
    while (m_ < n) {
        m = m_;
        for (j = 1; j <= m; j++) T_[j] = U_[j] + D_[j];
        D_[m + 1] = (seq_t)0;
        run = (seq_t)0;
        for (j = 1; j <= m + 1; j++) {
            tmp = D_[j];
            U_[j] = run;
            run += tmp;
        }
        run = (seq_t)0;
        for (j = m + 1; j >= 1; j--) {
            if (j <= m) run += T_[j];
            D_[j] = run;
        }
        tot = (seq_t)0;
        for (j = 1; j <= m + 1; j++) {
            tot += U_[j];
            tot += D_[j];
        }
        res_[m + 1] = tot;
        m_ = m + 1;
    }
    return res_[n] - 1;
}
