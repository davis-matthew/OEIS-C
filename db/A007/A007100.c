/*
 * A007100  Number of labeled trivalent (or cubic) 3-connected graphs with 2n
 *          nodes.
 * https://oeis.org/A007100
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)!*r(n)/(3*n*2^n), r(2) = 1, r(n) = (3*n-2)*(r(n-1) +
 *            Sum_{i=2..n-2} r(i)*r(n-i)). Substituting r(n) =
 *            3*n*2^n*a(n)/(2*n)! gives a recurrence without division: a(n) =
 *            (3*n-2)*((2*n-1)*(n-1)*a(n-1) + 3*Sum_{i=2..n-2}
 *            (n-i)*C(2*n-1,2*i-1)*a(i)*a(n-i)), a(2) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *A_tab = NULL;   /* A_tab[m] = a(m) */
static idx_t A_cap = 0;
static seq_t *P_row = NULL;   /* current Pascal row */
static idx_t P_cap = 0;
static idx_t P_idx = 0;       /* index of current Pascal row */
static idx_t A_done = 0;      /* largest m with a(m) computed */

static void advance_pascal(idx_t row) {
    if (P_cap < row + 1) {
        idx_t nc = row + 1 + P_cap;
        P_row = grow_zero(P_row, P_cap, nc);
        P_cap = nc;
    }
    for (idx_t k = P_idx + 1; k <= row; k++) {
        P_row[k] = 1;
        for (idx_t j = k - 1; j >= 1; j--)
            P_row[j] += P_row[j - 1];
    }
    if (row > P_idx) P_idx = row;
}

static void ensure_a(idx_t n) {
    if (n <= A_done) return;
    if (A_cap < n + 1) {
        idx_t nc = n + 1 + A_cap;
        A_tab = grow_zero(A_tab, A_cap, nc);
        A_cap = nc;
    }
    if (A_done == 0) {
        P_row = grow_zero(P_row, P_cap, 8);
        P_cap = 8;
        P_row[0] = 1;
        P_idx = 0;
        A_tab[2] = 1;
        A_done = 2;
    }
    for (idx_t m = A_done + 1; m <= n; m++) {
        advance_pascal(2 * m - 1);
        seq_t s = 0;
        for (idx_t i = 2; i + 2 <= m; i++) {
            s += (seq_t)(m - i) * P_row[2 * i - 1] * A_tab[i] * A_tab[m - i];
        }
        seq_t t = (seq_t)(2 * m - 1) * (seq_t)(m - 1) * A_tab[m - 1] + s * 3;
        A_tab[m] = (seq_t)(3 * m - 2) * t;
        A_done = m;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    ensure_a(n);
    return A_tab[n];
}
