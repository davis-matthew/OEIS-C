/*
 * A006604  Generalized Fibonacci numbers.
 * https://oeis.org/A006604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x)*(2 - x - x^2 + x^3) = 1 - x + f(x), f = g.f. of
 *            A006318 (large Schroeder S), i.e. 2*a(n) = S(n) + [n=0] - [n=1]
 *            + a(n-1) + a(n-2) - a(n-3); S(0)=1, S(1)=2, (n+1)*S(n) =
 *            3*(2n-1)*S(n-1) - (n-2)*S(n-2) (fixed-width builds use S(n) =
 *            S(n-1) + Sum_{k=0..n-1} S(k)*S(n-1-k) instead)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *S_tab = NULL;
static seq_t *A_tab = NULL;
static idx_t cap = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        S_tab = grow_zero(S_tab, cap, nc);
        A_tab = grow_zero(A_tab, cap, nc);
        cap = nc;
    }
    bool fw = seq_is_fixed_width();
    for (idx_t m = done; m <= n; m++) {
        if (m == 0) {
            S_tab[0] = 1;
        } else if (m == 1) {
            S_tab[1] = 2;
        } else if (!fw) {
            seq_t t = (seq_t)3 * (seq_t)(2 * m - 1) * S_tab[m - 1];
            t -= (seq_t)(m - 2) * S_tab[m - 2];
            S_tab[m] = t / (seq_t)(m + 1);
        } else {
            seq_t t = S_tab[m - 1];
            for (idx_t k = 0; k < m; k++)
                t += S_tab[k] * S_tab[m - 1 - k];
            S_tab[m] = t;
        }
        /* a(m) = (sum of terms)/2, computed as sum of halves plus carried low bits */
        seq_t h = S_tab[m] >> 1;
        int low = (int)(S_tab[m] & 1);
        if (m == 0) low += 1;
        if (m == 1) low -= 1;
        if (m >= 1) { h += A_tab[m - 1] >> 1; low += (int)(A_tab[m - 1] & 1); }
        if (m >= 2) { h += A_tab[m - 2] >> 1; low += (int)(A_tab[m - 2] & 1); }
        if (m >= 3) { h -= A_tab[m - 3] >> 1; low -= (int)(A_tab[m - 3] & 1); }
        if (low >= 0) h += (seq_t)(low / 2);
        else h -= (seq_t)((-low) / 2);
        A_tab[m] = h;
    }
    if (n + 1 > done) done = n + 1;
    return A_tab[n];
}
