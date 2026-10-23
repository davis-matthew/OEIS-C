/*
 * A186020  Eigentriangle of the binomial matrix.
 * https://oeis.org/A186020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Let T(n,k) be the eigentriangle of the binomial matrix
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 135, u64 n <= 350,
 *            u128 n <= 945, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For 0 ≤ k ≤ n it satisfies        T(n,k) = δ_{n,k} + Σ_{j=k}^{n-1} binomial(n-1,j)*T(j,k). */
/* The sequence a(n) is the row‑major concatenation of the rows of T:  */
/* a(0) = T(0,0), a(1) = T(1,0), a(2)=T(1,1), a(3)=T(2,0), …  */

#include <stdlib.h>
#include <math.h>
#include "../utils/binomial.h"

static seq_t **T_TABLE = NULL;      /* triangular table of T entries  */
static idx_t  T_ROWS   = 0;         /* number of rows already computed*/
static idx_t  T_CAP    = 0;         /* capacity of T_TABLE         */

/* Ensure that rows 0 … upto are computed. */
static void
ensure_rows(idx_t upto)
{
    if (upto < T_ROWS)
        return;                    /* already done */

    /* extend capacity if necessary */
    if (upto + 1 > T_CAP) {
        idx_t newcap = T_CAP > 0 ? T_CAP : 1;
        while (newcap <= upto + 1)
            newcap <<= 1;
        T_TABLE = (seq_t **)realloc(T_TABLE, newcap * sizeof(seq_t *));
        T_CAP  = newcap;
    }

    /* compute new rows */
    for (idx_t r = T_ROWS; r <= upto; ++r) {
        /* allocate row of length r+1 */
        seq_t *row = (seq_t *)calloc(r + 1, sizeof(seq_t));

        if (r > 0) {
            /* precompute binomial coefficients bin(r-1, j) for j=0..r-1 */
            seq_t *bin = (seq_t *)malloc(r * sizeof(seq_t));
            bin[0] = 1;
            for (idx_t j = 1; j < r; ++j)
                bin[j] = bin[j - 1] * ((seq_t)(r - 1 - (j - 1))) / (seq_t)j;

            /* fill row entries for k = 0 .. r-1 */
            for (idx_t k = 0; k < r; ++k) {
                seq_t sum = 0;
                for (idx_t j = k; j < r; ++j)
                    sum += bin[j] * T_TABLE[j][k];
                row[k] = sum;
            }
            free(bin);
        }

        /* diagonal element is 1 */
        row[r] = 1;

        T_TABLE[r] = row;
    }

    T_ROWS = upto + 1;
}

/* Convert the flat index n to the corresponding (row,r) and (column,c). */
static void
row_col_from_index(idx_t n, idx_t *r, idx_t *c)
{
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
    double sr = sqrt(1.0 + 8.0 * (double)n);
    idx_t  rr = (idx_t)((sr - 1.0) / 2.0);
    /* Adjust to guarantee the inequality */
    while (rr * (rr + 1) / 2 > n)
        rr--;
    while ((rr + 1) * (rr + 2) / 2 <= n)
        rr++;
    *r = rr;
    *c = n - rr * (rr + 1) / 2;
}

/* Public function required by the build system. */
seq_t
a(idx_t n)
{
    idx_t r, c;
    row_col_from_index(n, &r, &c);
    ensure_rows(r);
    return T_TABLE[r][c];
}
