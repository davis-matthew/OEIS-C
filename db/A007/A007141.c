/*
 * A007141  E-trees with at most 2 colors.
 * https://oeis.org/A007141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = E(2, n) where E(c, 1) = 1, E(c, n) = (1/(n-1)) *
 *            Sum_{i=1..n-1} ((c+1)*i - n + 1) * S(c, i) * E(c, n-i), S(c, n)
 *            = Sum_{d | n} E(c, d). Here c = 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *E_tab = NULL;
static seq_t *S_tab = NULL;
static idx_t have = 0;   /* E_tab[1..have], S_tab[1..have] are computed */
static idx_t cap = 0;    /* allocated length of the arrays */

static void ensure_computed(idx_t n) {
    if (n <= have) return;
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap : 16;
        while (newcap < n + 1) newcap *= 2;
        E_tab = grow_zero(E_tab, cap, newcap);
        S_tab = grow_zero(S_tab, cap, newcap);
        cap = newcap;
    }
    bool fixed = seq_is_fixed_width();
    for (idx_t m = have + 1; m <= n; m++) {
        if (m == 1) {
            E_tab[1] = 1;
            S_tab[1] = 1;
            continue;
        }
        idx_t d = m - 1;
        if (!fixed) {
            seq_t pos = 0, neg = 0;
            for (idx_t i = 1; i < m; i++) {
                seq_t p = S_tab[i] * E_tab[m - i];
                if (3 * i + 1 >= m) pos += p * (idx_t)(3 * i + 1 - m);
                else neg += p * (idx_t)(m - 3 * i - 1);
            }
            E_tab[m] = (pos - neg) / d;
        } else {
            /* split each product p = q*d + r so the division stays exact
               even when the full sum would wrap around */
            seq_t qp = 0, qn = 0;
            long long R = 0;
            for (idx_t i = 1; i < m; i++) {
                seq_t p = S_tab[i] * E_tab[m - i];
                seq_t q = p / d;
                long long r = (long long)(p % d);
                if (3 * i + 1 >= m) {
                    idx_t c = 3 * i + 1 - m;
                    qp += q * c;
                    R += (long long)c * r;
                } else {
                    idx_t c = m - 3 * i - 1;
                    qn += q * c;
                    R -= (long long)c * r;
                }
            }
            if (R >= 0) E_tab[m] = qp + (idx_t)(R / (long long)d) - qn;
            else E_tab[m] = qp - qn - (idx_t)((-R) / (long long)d);
        }
        seq_t s = 0;
        for (idx_t dv = 1; dv * dv <= m; dv++) {
            if (m % dv == 0) {
                s += E_tab[dv];
                if (dv != m / dv) s += E_tab[m / dv];
            }
        }
        S_tab[m] = s;
    }
    have = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_computed(n);
    return E_tab[n];
}
