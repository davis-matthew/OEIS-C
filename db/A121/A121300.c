/*
 * A121300  Triangle read by rows: T(n,k) is the number of directed
 *          column-convex polyominoes of area n and having k cells in the
 *          longest column (1<=k<=n).
 * https://oeis.org/A121300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS sequence A121300.
 * The sequence consists of the triangular array T(n,k) read in row
 * major order, where T(n,k) counts directed column‑convex polyominoes
 * of area n having longest column of height k.  The triangular
 * numbers are computed using the generating‑function formula
 * from the OEIS entry:
 *
 *   f[m](z) = (∑_{i=1..m} z^i) / (1 - ∑_{j=1..m} j z^j)
 *   T(n,k)  = [z^n] (f[k] - f[k-1])
 *
 * The implementation precomputes all needed f[m] and T[n][k] up to
 * the maximal row that covers the largest index requested, which
 * for n ≤ 10 000 is less than 150 rows.  The arrays are stored once
 * with static linkage; subsequent calls reuse the data.  Only the
 * operations allowed by the build (integer arithmetic and pointer
 * manipulation) are used.  The code is valid C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build, e.g.:
 *   typedef unsigned __int128 seq_t;
 *   typedef uint64_t idx_t;
 */

/* Forward declaration of the user‑visible function */
seq_t a(idx_t n);

/* -------------------------------------------------------------------------- */
/* Internal helper structures and constants                                 */
/* -------------------------------------------------------------------------- */

#define MAX_ROWS 150   /* sufficient for n <= 10 000 */

/* 2‑D array for f[m][n]   (m = 1..MAX_ROWS, n = 0..MAX_ROWS) */
static seq_t *f[MAX_ROWS + 1];

/* 2‑D array for T[n][k]   (n = 1..MAX_ROWS, k = 1..n) */
static seq_t *T[MAX_ROWS + 1];

static int initialized = 0;      /* one‑time initialisation flag */

/* -------------------------------------------------------------------------- */
/* Precomputation of f and T arrays                                          */
/* -------------------------------------------------------------------------- */

static void init_tables(void)
{
    if (initialized) return;
    initialized = 1;

    /* Allocate memory for f: f[m][n] */
    for (idx_t m = 0; m <= MAX_ROWS; m++)
    {
        f[m] = (seq_t *)malloc((MAX_ROWS + 1) * sizeof(seq_t));
        for (idx_t n = 0; n <= MAX_ROWS; n++)
            f[m][n] = (seq_t)0;
    }

    /* Allocate memory for T: T[n][k] (k runs from 1 to n) */
    for (idx_t n = 0; n <= MAX_ROWS; n++)
    {
        T[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= n; k++)
            T[n][k] = (seq_t)0;
    }

    /* Pre‑compute f[m] for all m = 1 .. MAX_ROWS */
    for (idx_t m = 1; m <= MAX_ROWS; m++)
    {
        /* Auxiliary array A for the reciprocal series 1/(1 - S_m(z)) */
        seq_t *A = (seq_t *)calloc((MAX_ROWS + 1), sizeof(seq_t));
        if (!A) exit(1);                            /* safety check */
        A[0] = (seq_t)1;

        /* Compute A[n] for n = 1 .. MAX_ROWS   (recurrence) */
        for (idx_t n = 1; n <= MAX_ROWS; n++)
        {
            seq_t sum = (seq_t)0;
            idx_t imax = (n < m) ? n : m;           /* i runs to min(n,m) */
            for (idx_t i = 1; i <= imax; i++)
                sum += (seq_t)i * A[n - i];
            A[n] = sum;
        }

        /* Compute f[m][n] = ∑_{i=1}^{min(n,m)} A[n-i]  for n ≥ 1 */
        for (idx_t n = 1; n <= MAX_ROWS; n++)
        {
            seq_t sum = (seq_t)0;
            idx_t imax = (n < m) ? n : m;
            for (idx_t i = 1; i <= imax; i++)
                sum += A[n - i];
            f[m][n] = sum;
        }

        free(A);
    }

    /* Compute the triangle entries T[n][k]   */
    for (idx_t n = 1; n <= MAX_ROWS; n++)
    {
        for (idx_t k = 1; k <= n; k++)
        {
            if (k == 1)
                T[n][k] = (seq_t)1;               /* T(n,1) = 1 for all n */
            else
                T[n][k] = f[k][n] - f[k - 1][n];
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Main function that returns the n-th term of the sequence                  */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    init_tables();                /* ensure tables are ready */

    /* Find the row 'row' so that triangular(row-1) < n ≤ triangular(row) */
    idx_t row = 1;
    while ((row * (row + 1)) / 2 < n)
        row++;

    idx_t prev_tri = ((row - 1) * row) / 2;   /* triangular(row-1) */
    idx_t k = n - prev_tri;                  /* column inside the row */

    return T[row][k];
}
