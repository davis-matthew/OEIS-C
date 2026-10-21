/*
 * A060850  Array of the coefficients A(n,k) in the expansion of
 *          Product_{i>=1} 1/(1-x^i)^n = Sum_{k>=0} A(n,k)*x^k, n >= 1, k >=
 *          0.
 * https://oeis.org/A060850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060850: coefficients A(n,k) of ∏_{i≥1} 1/(1‑x^i)^n, read by anti‑diagonals.
   a(n) = A(n_kinds,k) where n_kinds=k+m, k+m=⌈(√(8n+1)-1)/2⌉ and
   r=n‑(m‑1)m/2‑1,  n_kinds=m‑r, k=r.  Recurrence: A(0,0)=1,
   A(n,k)=∑_{m=0}^k partitions(m)·A(n‑1,k‑m). */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* static state for memoised values */
static idx_t curMaxS = 0;          /* largest diagonal already computed   */
static idx_t curCols  = 0;          /* number of columns ( = curMaxS )     */
static seq_t *table   = NULL;      /* flattened A[n][k], rows 0..S, cols 0..S‑1  */
static seq_t *part    = NULL;      /* partition numbers p[0..S]           */

/* Compute partition numbers up to 'max_n' (inclusive) into array 'part'.
   Uses the classic additive recurrence: p[0]=1; for i=1..max_n
   for n=i..max_n: p[n] += p[n-i].  */
static void compute_partitions(idx_t max_n, seq_t *part)
{
    part[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_n; ++i)
        for (idx_t n = i; n <= max_n; ++n)
            part[n] += part[n - i];
}

/* Ensure that all anti‑diagonals up to sum 's' (inclusive) are computed. */
static void ensure_up_to(idx_t s)
{
    if (s <= curMaxS)
        return;

    /* free previous data */
    free(table);
    free(part);

    curMaxS = s;
    curCols  = s;        /* columns 0..s-1 */

    /* allocate table: (s+1) rows × s columns */
    size_t rows = (size_t)(s + 1);
    size_t cols = (size_t)s;
    table = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    part  = (seq_t *)calloc(rows, sizeof(seq_t));

    /* compute partition numbers p[0..s] */
    compute_partitions(s, part);

    /* Base row n = 0: only A(0,0)=1, rest already zero */
    if (rows > 0 && cols > 0)
        table[0 * cols + 0] = (seq_t)1;

    /* compute rows for n = 1 .. s */
    for (idx_t n = 1; n <= s; ++n) {
        idx_t basePrev = (n - 1) * cols;
        idx_t baseCurr = n * cols;
        for (idx_t k = 0; k < cols; ++k) {
            seq_t sum = 0;
            for (idx_t m = 0; m <= k; ++m)
                sum += part[m] * table[basePrev + (k - m)];
            table[baseCurr + k] = sum;
        }
    }
}

/* Return the n‑th term of A060850 (1‑based index). */
seq_t a(idx_t n)
{
    /* find smallest s with s*(s+1)/2 >= n */
    long double root = sqrt((long double)(8.0L * n + 1.0L));
    idx_t s = (idx_t)((root - 1.0L) / 2.0L);
    while (s * (s + 1) / 2 < n)
        ++s;

    ensure_up_to(s);

    /* terms before current anti‑diagonal */
    idx_t prevTerms = (s - 1) * s / 2;
    idx_t r = n - prevTerms - 1;           /* 0‑based offset within diagonal */

    idx_t n_kinds = s - r;  /* row index (≥1) */
    idx_t k       = r;      /* column index (≥0) */

    return table[n_kinds * curCols + k];
}
