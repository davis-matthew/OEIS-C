/*
 * A001587  Generalized Euler numbers.
 * https://oeis.org/A001587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. cos(6x)*A(x) = sin(x)+sin(5x)+cos(x)+cos(5x), so a(n) =
 *            s(n)*(1+5^n) - Sum_{j>=1, 2j<=n} binomial(n,2j)*(-36)^j*a(n-2j),
 *            where s(n) = 1 if n mod 4 < 2, -1 otherwise; a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *A_vals = 0;
static seq_t *A_row = 0;
static seq_t *A_p36 = 0;
static idx_t A_cap = 0;
static idx_t A_cnt = 0;
static seq_t A_pow5 = 1;

seq_t a(idx_t n) {
    if (n < A_cnt) return A_vals[n];
    if (n + 1 > A_cap) {
        idx_t nc = A_cap ? A_cap : 16;
        while (nc < n + 1) nc *= 2;
        A_vals = grow_zero(A_vals, A_cap, nc);
        A_row = grow_zero(A_row, A_cap, nc);
        A_p36 = grow_zero(A_p36, A_cap, nc);
        A_cap = nc;
    }
    for (idx_t m = A_cnt; m <= n; m++) {
        /* update Pascal row to C(m, .) */
        A_row[m] = 1;
        if (m >= 1) {
            for (idx_t k = m - 1; k >= 1; k--) A_row[k] += A_row[k - 1];
        }
        A_row[0] = 1;
        if (m == 0) A_p36[0] = 1;
        else if ((m & 1) == 0) A_p36[m / 2] = A_p36[m / 2 - 1] * 36;

        seq_t g = A_pow5 + 1;
        A_pow5 *= 5;
        seq_t pos = 0, neg = 0;
        if ((m & 3) < 2) pos += g; else neg += g;
        for (idx_t j = 1; 2 * j <= m; j++) {
            seq_t t = A_row[2 * j] * A_p36[j] * A_vals[m - 2 * j];
            if (j & 1) pos += t; else neg += t;
        }
        A_vals[m] = pos - neg;
        A_cnt = m + 1;
    }
    return A_vals[n];
}
