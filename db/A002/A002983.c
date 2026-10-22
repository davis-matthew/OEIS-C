/*
 * A002983  Expansion of e.g.f. 1/(1 - x*exp(x) + x^2*exp(2*x)).
 * https://oeis.org/A002983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * f(k) * a(n-k), a(0) = 1,
 *            where f(k) = k - k*(k-1)*2^(k-2) for k >= 2 and f(1) = 1 (f(k) =
 *            k! [x^k] (x*exp(x) - x^2*exp(2*x)))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A_vals = NULL;   /* a(0..len-1) */
static seq_t *F_vals = NULL;   /* f(0..len-1) */
static seq_t *R_row = NULL;    /* binomial row for (len-1) */
static idx_t len = 0;
static idx_t cap = 0;
static seq_t pw = 0;           /* 2^(m-1) for last m processed */

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    seq_t *na = (seq_t *)malloc(nc * sizeof(seq_t));
    seq_t *nf = (seq_t *)malloc(nc * sizeof(seq_t));
    seq_t *nr = (seq_t *)malloc(nc * sizeof(seq_t));
    for (idx_t i = 0; i < len; i++) {
        na[i] = A_vals[i];
        nf[i] = F_vals[i];
        nr[i] = R_row[i];
    }
    free(A_vals);
    free(F_vals);
    free(R_row);
    A_vals = na;
    F_vals = nf;
    R_row = nr;
    cap = nc;
}

seq_t a(idx_t n) {
    if (n < len) return A_vals[n];
    ensure_cap(n + 1);
    while (len <= n) {
        idx_t m = len;
        if (m == 0) {
            A_vals[0] = 1;
            F_vals[0] = 0;
            R_row[0] = 1;
        } else {
            /* advance binomial row from m-1 to m (additions only) */
            R_row[m] = 1;
            for (idx_t j = m - 1; j >= 1; j--)
                R_row[j] += R_row[j - 1];
            /* f(m) = m - (m(m-1)/2) * 2^(m-1) */
            if (m == 1) pw = 1;
            else pw += pw;
            idx_t tri = (m % 2 == 0) ? (m / 2) * (m - 1) : m * ((m - 1) / 2);
            F_vals[m] = (seq_t)m - (seq_t)tri * pw;
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++)
                s += R_row[k] * F_vals[k] * A_vals[m - k];
            A_vals[m] = s;
        }
        len++;
    }
    return A_vals[n];
}
