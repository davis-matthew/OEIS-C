/*
 * A220222  Triangular array read by rows. T(n,k) is the number of functional
 *          digraphs on {1,2,...,n} such that no node is at a distance greater
 *          than one from a cycle and there are k recurrent elements whose
 *          preimage contains only one element, n>=0, 0<=k<=n.
 * https://oeis.org/A220222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are determined by the index * of the
 *            triangular array read by rows; T(r,k) = number of functional *
 *            digraphs on {1,…,r} satisfying the distance‑≤1 condition with
 *            exactly * k recurrent elements whose preimage contains only that
 *            element
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 51, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* maximal row that has to be handled.  For flat index up to about 10 000
 * we need row r ≤ 200 (200*201/2 ≈ 20 300). */
#define MAX_ROW 200

/* pre‑computed factorials 0! … MAX_ROW! */
static seq_t fact[MAX_ROW + 1];

/* Stirling numbers of the second kind S[n][k] for 0 ≤ n,k ≤ MAX_ROW. */
static seq_t stirling[MAX_ROW + 1][MAX_ROW + 1];

/* flag indicating that the tables have been filled */
static bool tables_ready = false;

/* initialise factorials and Stirling numbers once */
static void init_tables(void)
{
    if (tables_ready) return;

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_ROW; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Stirling numbers (second kind) */
    for (idx_t i = 0; i <= MAX_ROW; ++i)
        for (idx_t j = 0; j <= MAX_ROW; ++j)
            stirling[i][j] = (seq_t)0;

    stirling[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            if (k == n)
                stirling[n][k] = (seq_t)1;
            else
                stirling[n][k] = k * stirling[n - 1][k] + stirling[n - 1][k - 1];
        }
    }
    tables_ready = true;
}

/* T(n,k) – number of functional digraphs on {1,…,n} with the given
 * properties.  Formula taken from the OEIS entry:
 *
 * T(n,k) = Σ_{c = k..n}   n! * c! / ((n-c)! * k!)  * S(n-c, c-k)
 *
 * where S is Stirling number of the second kind.
 */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t sum = (seq_t)0;

    for (idx_t c = k; c <= n; ++c) {
        idx_t L = n - c;          /* number of leaf nodes */
        idx_t m = c - k;          /* cycle nodes that receive leaves */

        /* S(L, m) is zero unless m ≤ L */
        if (m > L) continue;

        seq_t term = fact[n];            /* n!  */
        term     *= fact[c];            /* * c!     */
        term     /= fact[n - c];        /* / (n-c)! */
        term     /= fact[k];            /* / k!     */
        term     *= stirling[L][m];     /* * S(L, m) */

        sum += term;
    }
    return sum;
}

/* Main task function; returns the nth term of the sequence A220222 */
seq_t a(idx_t idx)
{
    init_tables();

    /* locate row r and column k of the triangle that contains a[idx] */
    idx_t r = 0;
    if (idx > 0) {
        /* solve r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2  */
        long double d = sqrtl(8.0L * (long double)idx + 1.0L);
        r = (idx_t)((d - 1.0L) / 2.0L);

        /* correct for possible rounding errors of the floating point calculation */
        while ((r + 1) * (r + 2) / 2 <= idx) ++r;
        while (r * (r + 1) / 2 > idx) --r;
    }
    idx_t k = idx - r * (r + 1) / 2;

    return T_value(r, k);
}
