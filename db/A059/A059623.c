/*
 * A059623  As upper right triangle, number of weakly unimodal partitions of n
 *          where initial part is k (n >= k >= 1).
 * https://oeis.org/A059623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059623: weakly unimodal partitions
 *
 *  The triangle T(n,k) (1 ≤ k ≤ n) satisfies
 *      T(n,k) = S(n,k) - S(n-k,k) + Σ_{j=k}^{n-k} T(n-k,j)
 *
 *  where S(n,k) is the number of partitions of n into exactly k parts,
 *      S(n,k) = S(n-1,k-1) + S(n-k,k)   (S(0,0)=1, S(t,0)=0 for t>0).
 *
 *  The single sequence a(m) is the triangle read one row at a time:
 *      m = 1          → T(1,1)
 *      m = 2          → T(2,1)
 *      m = 3          → T(2,2)
 *      m = 4          → T(3,1)
 *      ...
 *  Thus a(m) = T(row, col) where row is the smallest integer with
 *  row*(row+1)/2 ≥ m and col = m - (row-1)*row/2.
 */

/* required standard header */
#include <stdlib.h>

/* static tables for S, T and suffix sums of T */
static seq_t **S = NULL;          /* S[n][k] */
static seq_t **T = NULL;          /* T[n][k] */
static seq_t **Suff = NULL;       /* Suff[n][k] = Σ_{j=k}^{n} T[n][j] */
static idx_t max_computed = 0;    /* highest n already computed */

/* expand tables to include rows up to n */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;

    /* allocate or enlarge pointer arrays */
    if (max_computed == 0) {
        S     = (seq_t**)malloc(sizeof(seq_t*) * (n + 1));
        T     = (seq_t**)malloc(sizeof(seq_t*) * (n + 1));
        Suff  = (seq_t**)malloc(sizeof(seq_t*) * (n + 1));
    } else {
        S     = (seq_t**)realloc(S,     sizeof(seq_t*) * (n + 1));
        T     = (seq_t**)realloc(T,     sizeof(seq_t*) * (n + 1));
        Suff  = (seq_t**)realloc(Suff,  sizeof(seq_t*) * (n + 1));
    }

    /* initialise row 0 (used for the recurrence) */
    if (!S[0]) {
        S[0]    = (seq_t*)calloc(1, sizeof(seq_t));
        S[0][0] = (seq_t)1;
    }
    if (!T[0]) {
        T[0]    = (seq_t*)calloc(1, sizeof(seq_t));
        T[0][0] = (seq_t)0;
    }
    if (!Suff[0]) {
        Suff[0] = (seq_t*)calloc(2, sizeof(seq_t));   /* index 0..1 */
        Suff[0][1] = (seq_t)0;
    }

    /* compute new rows */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* allocate row i */
        S[i]    = (seq_t*)calloc((size_t)i + 1, sizeof(seq_t));
        T[i]    = (seq_t*)calloc((size_t)i + 1, sizeof(seq_t));
        Suff[i] = (seq_t*)calloc((size_t)i + 2, sizeof(seq_t));  /* +1 sentinel */

        /* S(n,k) for k = 1..i */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t val = S[i-1][k-1];              /* S(n-1,k-1)   */
            if (i >= k && (i - k) >= k)          /* add S(n-k,k) if defined */
                val += S[i-k][k];
            S[i][k] = val;
        }

        /* T(n,k) for k = 1..i */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t val = 0;
            if (i >= k) {
                seq_t s1  = S[i][k];
                seq_t s2  = 0;
                seq_t sum = 0;

                if (i >= k && (i - k) >= k) {
                    s2  = S[i-k][k];
                    sum = Suff[i-k][k];
                }

                val = s1 - s2 + sum;
            }
            T[i][k] = val;
        }

        /* suffix sums for row i */
        Suff[i][i+1] = (seq_t)0;
        for (idx_t k = i; k > 0; --k) {
            Suff[i][k] = Suff[i][k+1] + T[i][k];
        }
    }
    max_computed = n;
}

/* determine the row index for linear position n */
static idx_t row_of(idx_t pos)
{
    idx_t m = 1;
    while (m * (m + 1) / 2 < pos) m++;
    return m;
}

/* public routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* map linear index to (row, col) */
    idx_t row = row_of(n);
    idx_t prev_cnt = (row - 1) * row / 2;
    idx_t col = n - prev_cnt;             /* 1‑based column */

    compute_up_to(row);                   /* ensure the required row is present */
    return T[row][col];
}
