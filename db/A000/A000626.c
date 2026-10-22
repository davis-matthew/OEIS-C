/*
 * A000626  Number of stereoisomeric paraffins with n carbon atoms.
 * https://oeis.org/A000626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000628(n) - N(n), where N(n) counts constitutional
 *            alkanes in which no carbon has four distinct substituents (H
 *            counted as an empty branch). Steric alkyls (A000625): S(x) = 1 +
 *            x*(S(x)^3 + 2*S(x^3))/3, S(0) = 1. With e2(U) = (U(x)^2 -
 *            U(x^2))/2 and h2(U) = (U(x)^2 + U(x^2))/2: A000628(n)...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 26, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   a626_ch2, a626_div3, a626_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a626_ch2.h"
#include "../utils/a626_div3.h"
#include "../utils/a626_realloc.h"

static seq_t *S_ = NULL, *P_ = NULL, *E_ = NULL, *H_ = NULL, *R_ = NULL;
static idx_t cap_ = 0, len_ = 0;

static void a626_grow(idx_t need)
{
    idx_t k, i;
    if (need < len_) return;
    if (need + 1 > cap_) {
        idx_t nc = cap_ * 2;
        if (nc < need + 1) nc = need + 1;
        if (nc < 16) nc = 16;
        S_ = a626_realloc(S_, len_, nc);
        P_ = a626_realloc(P_, len_, nc);
        E_ = a626_realloc(E_, len_, nc);
        H_ = a626_realloc(H_, len_, nc);
        R_ = a626_realloc(R_, len_, nc);
        cap_ = nc;
    }
    for (k = len_; k <= need; k++) {
        /* steric alkyls */
        if (k == 0) {
            S_[0] = 1;
        } else {
            idx_t j = k - 1;
            seq_t t = 0;
            for (i = 0; i <= j; i++) t += P_[i] * S_[j - i];
            if (j % 3 == 0) { t += S_[j / 3]; t += S_[j / 3]; }
            S_[k] = a626_div3(t);
        }
        /* S^2, e2(S), h2(S) */
        {
            seq_t base = 0;
            for (i = 0; 2 * i < k; i++) base += S_[i] * S_[k - i];
            P_[k] = base + base;
            E_[k] = base;
            H_[k] = base;
            if (k % 2 == 0) {
                seq_t c = S_[k / 2];
                P_[k] += c * c;
                E_[k] += a626_ch2(c);
                H_[k] += a626_ch2(c + 1);
            }
        }
        /* restricted constitutional alkyls */
        if (k == 0) {
            R_[0] = 1;
        } else {
            seq_t t = 0;
            idx_t j;
            for (j = 0; 2 * j <= k - 1; j++) t += R_[j] * R_[k - 1 - 2 * j];
            R_[k] = t;
        }
    }
    len_ = need + 1;
}

/* pair sums over R truncated to degree m */
static seq_t a626_pairR(idx_t k, idx_t m, int plus)
{
    seq_t base = 0;
    idx_t i = (k > m) ? k - m : 0;
    for (; 2 * i < k; i++) base += R_[i] * R_[k - i];
    if (k % 2 == 0 && k / 2 <= m) {
        if (plus) base += a626_ch2(R_[k / 2] + 1);
        else base += a626_ch2(R_[k / 2]);
    }
    return base;
}

seq_t a(idx_t n)
{
    idx_t i, j, m, k0;
    seq_t tot = 0, N = 0;
    if (n < 1) return 0;
    a626_grow(n);

    /* A000628(n) */
    for (i = 0; i <= n - 1; i++) tot += S_[i] * S_[n - i];
    for (i = 0; i <= n - 1; i++) tot -= E_[i] * H_[n - 1 - i];
    for (i = 1; 2 * i < n; i++) tot -= S_[i] * S_[n - i];
    if (n % 2 == 0) tot -= a626_ch2(S_[n / 2]);

    /* non-stereoisomeric constitutional alkanes */
    m = (n - 1) / 2;
    k0 = n - 1;
    for (j = 0; 2 * j <= k0 && j <= m; j++)
        N += R_[j] * a626_pairR(k0 - 2 * j, m, 1);
    if (k0 % 2 == 0) N -= a626_pairR(k0 / 2, m, 0);
    if (n % 2 == 0) N += a626_ch2(R_[n / 2] + 1);

    return tot - N;
}
