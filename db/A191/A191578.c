/*
 * A191578  Triangle read by rows, based on expansion of (x^2/(exp(x)-1))^m =
 *          x^m+sum(n>m T(n,m)*m!/((n-m)!*n!)*x^n).
 * https://oeis.org/A191578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row, col) where row and col are obtained from the order
 *            reading of the triangle (x^2/(exp(x)-1))^m. We evaluate the
 *            explicit formula T(n,m) = n! * Σ_{k=0}^{n-m} (k! * S1(m+k,m) *
 *            S2(n-m,k)) / (m+k)!, where S1 are signed Stirling numbers of the
 *            first kind and S2 are Stirling numbers of th...
 * Range:     i8 n <= 16, i16 n <= 28, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_R 200

static seq_t fact[MAX_R + 1];
static seq_t s1[MAX_R + 1][MAX_R + 1]; /* signed Stirling numbers of the first kind */
static seq_t s2[MAX_R + 1][MAX_R + 1]; /* Stirling numbers of the second kind */
static int tables_initialized = 0;

static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = 1;

    /* factorials */
    fact[0] = 1;
    for (int i = 1; i <= MAX_R; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* signed Stirling numbers of the first kind */
    for (int n = 0; n <= MAX_R; ++n)
        for (int k = 0; k <= n; ++k) {
            if (n == 0 && k == 0)
                s1[n][k] = 1;
            else if (k == 0)
                s1[n][k] = 0;
            else {
                seq_t t1 = s1[n - 1][k - 1];
                seq_t t2 = s1[n - 1][k] * (seq_t)(n - 1);
                s1[n][k] = t1 - t2;
            }
        }

    /* Stirling numbers of the second kind */
    for (int n = 0; n <= MAX_R; ++n)
        for (int k = 0; k <= n; ++k) {
            if (n == 0 && k == 0)
                s2[n][k] = 1;
            else if (k == 0)
                s2[n][k] = 0;
            else {
                seq_t t1 = s2[n - 1][k] * (seq_t)k;
                seq_t t2 = s2[n - 1][k - 1];
                s2[n][k] = t1 + t2;
            }
        }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑indexed */

    /* Determine the row (r) and column (c) of the triangle from the
       linear index n.                     */
    idx_t r = 1, c = n;
    while (c > r) {
        c -= r;
        ++r;
    }

    if (r > (idx_t)MAX_R || c > (idx_t)MAX_R) {
        /* Outside the pre‑computed range – return 0.    */
        return 0;
    }

    init_tables();

    size_t r_s = (size_t)r;
    size_t c_s = (size_t)c;
    size_t diff = r_s - c_s;          /* n - m */

    seq_t result = 0;
    for (size_t k = 0; k <= diff; ++k) {
        seq_t term = fact[r_s] / fact[c_s + k];   /* exact division */
        term *= fact[k];
        term *= s1[c_s + k][c_s];
        term *= s2[diff][k];
        result += term;
    }
    return result;
}
