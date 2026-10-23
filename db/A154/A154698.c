/*
 * A154698  Triangular sequence T(n, m) = (p^(n-m)*q^m + p^m*q^(n-m))*A(n+1,
 *          m+1), where A(n, m) = (3*n -3*k +1)A(n-1, k-1) + (3*k-2)A(n-1, k),
 *          A(n,1)=A(n,n)=1, p=2 and q=3.
 * https://oeis.org/A154698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^(n-m) * 3^m + 2^m * 3^(n-m)) * A(n+1, m+1), where
 *            A(1,1)=1, A(n,1)=A(n,n)=1, and for 1 < m < n A(n,m) = (3*n - 3*m
 *            + 1) * A(n-1, m-1) + (3*m - 2) * A(n-1, m). Initial seeds for
 *            the flattened sequence are a(0) = 2, a(1) = 5, a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 37, u64 n <= 94, u128 n <= 230,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* --------- small helper: compute X^e for small integer base X (2 or 3) --------- */
static seq_t
pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp != 0) {
        if (exp & 1U)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* --------- table of the A‑triangle, rows indexed from 1 (A(1,*)) --------- */
static seq_t **A_rows = NULL;   /* array of row pointers, A_rows[0] unused */
static idx_t  max_row = 0;      /* highest row index that has been allocated */

/* creates A-row i: i>=1*/
static void
allocate_row(idx_t i)
{
    seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
    if (!row)
        abort();                 /* fatal: out of memory */

    row[0] = 0;                  /* unused, keeps indices 1..i */
    row[1] = row[i] = 1;          /* A(i,1)=A(i,i)=1 */

    if (i > 1) {
        for (idx_t m = 2; m < i; ++m) {
            seq_t term1 = ((seq_t)(3 * i - 3 * m + 1)) * A_rows[i - 1][m - 1];
            seq_t term2 = ((seq_t)(3 * m - 2))     * A_rows[i - 1][m];
            row[m] = term1 + term2;
        }
    }

    A_rows[i] = row;
    if (i > max_row)
        max_row = i;
}

/* make sure rows 1..need are allocated */
static void
ensure_rows(idx_t need)
{
    if (need <= max_row)
        return;

    A_rows = (seq_t **)realloc(A_rows, (size_t)(need + 1) * sizeof(seq_t *));
    if (!A_rows)
        abort();

    for (idx_t i = max_row + 1; i <= need; ++i)
        A_rows[i] = NULL;

    for (idx_t i = max_row + 1; i <= need; ++i)
        allocate_row(i);
}

/* --------- main sequence function --------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Find the row index r such that triangular(r) <= n < triangular(r+1). */
    /* For n <= 10000 this calculation is safe with double precision.  */
    idx_t r = (idx_t)((sqrt((double)(8.0 * n + 1.0)) - 1.0) / 2.0);

    /* Correct any floating‑point inaccuracy. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    /* Column index (0‑based) and 1‑based A‑indices. */
    idx_t col0 = n - r * (r + 1) / 2;         /* n - triangular(r) */
    idx_t col1 = col0 + 1;                   /* 1‑based for A */

    /* Ensure A rows up to r+1 exist (because A(n+1,*)). */
    ensure_rows(r + 1);

    /* Compute the factor (2^(r-col0)*3^col0 + 2^col0*3^(r-col0)). */
    idx_t exp1 = r - col0;        /* exponent for 2 in the first term */
    idx_t exp2 = col0;            /* exponent for 3 in the first term */

    seq_t part1 = pow_uint(2, exp1) * pow_uint(3, exp2);
    seq_t part2 = pow_uint(2, exp2) * pow_uint(3, exp1);
    seq_t factor = part1 + part2;

    /* Final result: factor multiplied by A(r+1, col1). */
    return factor * A_rows[r + 1][col1];
}
