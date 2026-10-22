/*
 * A006569  Numerators of generalized Bernoulli numbers.
 * https://oeis.org/A006569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = numerator(B(n)), where E.g.f. Sum B(n) x^n/n! = x^2/2 /
 *            (e^x-1-x), i.e. B(0) = 1, B(m) = -Sum_{k=1..m} binomial(m,k) *
 *            B(m-k) / ((k+1)*(k+2)/2)
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 17, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   frac_gcd, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/frac_gcd.h"

static seq_t *Bn = NULL;   /* numerators */
static seq_t *Bd = NULL;   /* denominators (positive) */
static seq_t *row = NULL;  /* Pascal row for current m */
static idx_t B_len = 0;
static idx_t B_cap = 0;

static void compute_up_to(idx_t n) {
    idx_t m, k;
    if (B_len > n) return;

    if (B_cap < n + 1) {
        idx_t nc = B_cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        Bn = grow_zero(Bn, B_cap, nc);
        Bd = grow_zero(Bd, B_cap, nc);
        row = grow_zero(row, B_cap, nc);
        B_cap = nc;
    }

    if (B_len == 0) {
        Bn[0] = 1;
        Bd[0] = 1;
        row[0] = 1;
        B_len = 1;
    }

    for (m = B_len; m <= n; m++) {
        seq_t sn = 0, sd = 1;

        /* advance Pascal row from m-1 to m */
        row[m] = 0;
        for (k = m; k >= 1; k--) row[k] += row[k - 1];

        for (k = 1; k <= m; k++) {
            seq_t N = Bn[m - k], D = Bd[m - k];
            seq_t c, q, g, tn, td;
            if (N == 0) continue;
            c = row[k];
            q = ((seq_t)k + 1) * ((seq_t)k + 2) / 2;
            g = frac_gcd(c, q);
            c /= g; q /= g;
            g = frac_gcd(c, D);
            c /= g; D /= g;
            g = frac_gcd(N, q);
            N /= g; q /= g;
            tn = N * c;
            td = D * q;
            /* sn/sd += tn/td */
            g = frac_gcd(sd, td);
            {
                seq_t td_g = td / g;
                seq_t sd_g = sd / g;
                seq_t num = sn * td_g + tn * sd_g;
                seq_t den = sd * td_g;
                if (num == 0) {
                    sn = 0;
                    sd = 1;
                } else {
                    seq_t h = frac_gcd(num, den);
                    sn = num / h;
                    sd = den / h;
                }
            }
        }
        Bn[m] = -sn;
        Bd[m] = sd;
    }

    B_len = n + 1;
}

seq_t a(idx_t n) {
    compute_up_to(n);
    return Bn[n];
}
