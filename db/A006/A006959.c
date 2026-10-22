/*
 * A006959  Number of labeled M-type rooted trees on n nodes.
 * https://oeis.org/A006959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen, nice
 * Formula:   with A(x) the e.g.f. (a(0)=0), E(x) = exp(x+A(x)) = 1 +
 *            (2+log(1-x))*A(x), L(x) = -log(1-x): A'(x) = E(x) + A(x)/(1-x) +
 *            (L(x) + E(x) - 1)*A'(x). Coefficientwise (e(0)=1, e(m+1) =
 *            Sum_{i=0..m} binomial(m,i)*([i=0]+a(i+1))*e(m-i)): a(n) = e(n-1)
 *            + Sum_{i=0..n-2} (n-1)!/(n-1-i)! * a(n-1-i) + Sum_{i...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *A_ = 0, *E_ = 0, *F_ = 0, *R_ = 0;
static idx_t cap_ = 0, done_ = 0;

static void ensure_cap(idx_t need)
{
    idx_t nc;
    if (need <= cap_) return;
    nc = cap_ * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    A_ = grow_zero(A_, cap_, nc);
    E_ = grow_zero(E_, cap_, nc);
    F_ = grow_zero(F_, cap_, nc);
    R_ = grow_zero(R_, cap_, nc);
    cap_ = nc;
}

seq_t a(idx_t n)
{
    idx_t k, i, j;
    if (n < 1) return 0;
    ensure_cap(n + 2);
    if (done_ == 0) {
        A_[0] = 0;
        E_[0] = 1;
        F_[0] = 1;
        R_[0] = 1;
    }
    for (k = done_ + 1; k <= n; k++) {
        seq_t val, ff;
        F_[k] = F_[k - 1] * (seq_t)k;
        if (k >= 2) {
            seq_t s = 0;
            /* R_ currently holds binomial row k-2 */
            for (i = 0; i <= k - 2; i++) {
                seq_t t = A_[i + 1];
                if (i == 0) t += 1;
                s += R_[i] * t * E_[k - 2 - i];
            }
            E_[k - 1] = s;
            /* advance to binomial row k-1 */
            R_[k - 1] = 0;
            for (j = k - 1; j >= 1; j--) R_[j] += R_[j - 1];
        }
        val = E_[k - 1];
        ff = 1;
        if (k >= 2) {
            for (i = 0; i <= k - 2; i++) {
                val += ff * A_[k - 1 - i];
                ff *= (seq_t)(k - 1 - i);
            }
        }
        for (i = 1; i <= k - 1; i++)
            val += R_[i] * (F_[i - 1] + E_[i]) * A_[k - i];
        A_[k] = val;
        done_ = k;
    }
    return A_[n];
}
