/*
 * A058942  Triangle of coefficients of Gandhi polynomials.
 * https://oeis.org/A058942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) = coefficient of x^{k-1} in the Gandhi polynomial C_r(x)
 *            where r and k are determined by the triangular numbering: n <=
 *            r(r+1)/2 and k = n - (r-1)r/2 . The polynomials satisfy:
 *            C_1(x)=1, C_n(x)=(x+1)*((x+1)*C_{n-1}(x+1)-x*C_{n-1}(x))
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 121,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **rows = NULL;   /* rows[1..rows_max] : arrays of coefficients */
static idx_t rows_max = 0;    /* largest row computed so far */

/* Compute binomial coefficient C(i, j) as seq_t, i >= j >= 0 */
static seq_t binom_seq(idx_t i, idx_t j)
{
    seq_t bin = 1;
    for (idx_t k = 0; k < j; ++k)
    {
        bin = bin * (i - k) / (k + 1);
    }
    return bin;
}

/* Ensure that rows up to needed_r are computed */
static void ensure_rows(idx_t needed_r)
{
    if (needed_r <= rows_max) return;

    /* Allocate new space for rows array */
    rows = (seq_t**)realloc(rows, sizeof(seq_t*) * (needed_r + 1));
    /* Compute rows iteratively from rows_max+1 up to needed_r */
    for (idx_t r = rows_max + 1; r <= needed_r; ++r)
    {
        if (r == 1)
        {
            /* C_1(x) = 1 */
            rows[1] = (seq_t*)malloc(sizeof(seq_t));  /* length 1 */
            rows[1][0] = 1;
            continue;
        }

        idx_t L = r - 1;                     /* length of previous row */
        seq_t *prev = rows[r - 1];            /* previous coefficients */
        seq_t *b = (seq_t*)calloc(L, sizeof(seq_t)); /* C_{r-1}(x+1) coeffs */

        /* Compute b[j] = sum_{i=j}^{L-1} prev[i] * C(i,j) */
        for (idx_t i = 0; i < L; ++i)
        {
            seq_t bin = 1;
            for (idx_t j = 0; j <= i; ++j)
            {
                b[j] += prev[i] * bin;
                /* next binomial coefficient */
                if (j < i)
                    bin = bin * (i - j) / (j + 1);
            }
        }

        /* Allocate new row array */
        seq_t *curr = (seq_t*)malloc(sizeof(seq_t) * r);
        for (idx_t k = 0; k < r; ++k)
        {
            seq_t term = 0;
            /* b_k */
            if (k < L)
                term += b[k];
            /* 2*b_{k-1} */
            if (k >= 1 && k - 1 < L)
                term += b[k - 1] * 2;
            /* b_{k-2} */
            if (k >= 2 && k - 2 < L)
                term += b[k - 2];
            /* -a_{k-1} */
            if (k >= 1 && k - 1 < L)
                term -= prev[k - 1];
            /* -a_{k-2} */
            if (k >= 2 && k - 2 < L)
                term -= prev[k - 2];
            curr[k] = term;
        }

        free(b);
        rows[r] = curr;
    }

    rows_max = needed_r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but handle gracefully */
        return 0;

    /* Find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n )
        ++r;

    /* Column k (1-based) */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;     /* 1 <= k <= r */

    ensure_rows(r);

    return rows[r][k - 1];
}
