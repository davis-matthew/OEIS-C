/*
 * A214622  Triangle read by rows, matrix inverse of
 *          [x^(n-k)](skp(n,x)-skp(n,x-1)+x^n) where skp denotes the
 *          Swiss-Knife polynomials A153641.
 * https://oeis.org/A214622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = f(r - k) * C(r, k) where r and k are the row and column
 *            indices of the flattened triangle, f[d] are the first‑column
 *            entries (T(d,0)), and C(n,k) is the binomial coefficient. The
 *            first‑column entries are obtained from two nested recurrences
 *            derived from the generating function of the sequence
 * Range:     i8 n <= 14, i16 n <= 35, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for the utility functions that we use */
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t */
#include "../utils/pow2.h"       /* pow2(idx_t exp) -> seq_t */

/* Maximum number of rows we pre‑compute (sufficient for n up to about 10000). */
#define MAX_ROWS 20000

/* Static tables for the recurrences. */
static seq_t a_table[MAX_ROWS];   /* a[n] – integer coefficients of the A(x) e.g.f. */
static seq_t f_table[MAX_ROWS];   /* f[n] – first‑column entries (T(n,0)). */
static size_t computed_rows = 0;  /* Highest index already computed (inclusive). */

/* Helper: compute a[n] and f[n] up to the required index. */
static void ensure_computed(size_t up_to)
{
    if (up_to < computed_rows)
        return;                     /* already computed */

    if (computed_rows == 0) {
        a_table[0] = 1;
        f_table[0] = 1;
        computed_rows = 0;
    }

    for (size_t n = computed_rows + 1; n <= up_to; ++n) {
        /* ---- Compute a[n] ---- */
        seq_t sum_a = 0;
        for (size_t i = 1; i < n; ++i) {
            seq_t bin = binomial((idx_t)n, (idx_t)i);
            seq_t p2  = pow2((idx_t)(n - i));
            sum_a += bin * a_table[i] * p2;
        }
        seq_t temp = (seq_t)2 - sum_a;
        a_table[n] = temp / (seq_t)2;   /* always divisible by 2 */

        /* ---- Compute f[n] ---- */
        seq_t sum_f = 0;
        for (size_t k = 1; k <= n; ++k) {
            seq_t bin = binomial((idx_t)n, (idx_t)k);
            sum_f += bin * a_table[k] * f_table[n - k];
        }
        f_table[n] = -sum_f;
    }

    computed_rows = up_to;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    /* Determine the row r such that the cumulative count before row r
       is <= n < cumulative count before row r+1.  The cumulative count
       before row r is r*(r+1)/2. */
    idx_t r = 0;
    idx_t total = 0;
    while (total + r + 1 <= n) {
        total += r + 1;
        ++r;
    }
    idx_t k = n - total;   /* column index within the row */

    /* Ensure the tables are computed up to the required row. */
    ensure_computed((size_t)r);

    /* a(n) = f[r - k] * C(r, k) */
    seq_t bin = binomial((idx_t)r, (idx_t)k);
    return f_table[r - k] * bin;
}
