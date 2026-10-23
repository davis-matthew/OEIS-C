/*
 * A178116  Coefficient array for polynomials
 *          P(n,x)=x*P(n-1,x)+floor(n^2/4)*P(n-2,x), P(0,x)=1,P(1,x)=x.
 * https://oeis.org/A178116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: P(0,x)=1, P(1,x)=x,
   P(n,x)=x·P(n-1,x)+⌊n²/4⌋·P(n-2,x).
   a(n) is the coefficient of x¹ in P(n,x).  Thus
   a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=0, a(5)=1, ...  */

/* ANSI C / C++17 compatible */

#include <stddef.h>
#include <stdlib.h>

/* static cache of polynomial coefficient rows */
static seq_t **coeff_rows = NULL;   /* rows[0]..rows[r] contain P(r,x) */
static idx_t  coeff_row_count = 0;  /* number of rows already computed (== rows[0..coeff_row_count-1] exist) */

/* ensure rows 0 .. r_need are computed and stored */
static void ensure_coeff_rows(idx_t r_need)
{
    if (coeff_row_count > r_need)
        return;                                 /* already computed */

    /* enlarge the rows pointer array if needed */
    if (coeff_row_count == 0) {
        coeff_rows = (seq_t **)calloc(1, sizeof(seq_t *));
    } else {
        coeff_rows = (seq_t **)realloc(coeff_rows,
                                        (size_t)(r_need + 1) * sizeof(seq_t *));
    }

    /* compute new rows sequentially */
    for (idx_t r = coeff_row_count; r <= r_need; ++r) {
        /* row length is r+1: coefficients of x^0 .. x^r */
        coeff_rows[(size_t)r] = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        if (r == 0) {
            coeff_rows[0][0] = 1;                /* P(0,x)=1 */
        } else if (r == 1) {
            coeff_rows[1][0] = 0;                /* P(1,x)=x */
            coeff_rows[1][1] = 1;
        } else {
            /* compute floor(r²/4) */
            seq_t factor = (seq_t)r * (seq_t)r / (seq_t)4;
            for (idx_t k = 0; k <= r; ++k) {
                seq_t val = 0;
                if (k > 0)                                 /* x·P(n-1) term */
                    val += coeff_rows[(size_t)(r - 1)][(size_t)(k - 1)];
                if (k <= r - 2)                           /* ⌊n²/4⌋·P(n-2) term */
                    val += factor * coeff_rows[(size_t)(r - 2)][(size_t)k];
                coeff_rows[(size_t)r][(size_t)k] = val;
            }
        }
    }
    coeff_row_count = r_need + 1;
}

/* mapping n to (row r, column k) in the triangle */
static void row_col(idx_t n, idx_t *r_out, idx_t *k_out)
{
    /* find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    *r_out = r;
    *k_out = n - r * (r + 1) / 2;
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col(n, &r, &k);

    ensure_coeff_rows(r);

    return coeff_rows[(size_t)r][(size_t)k];
}
