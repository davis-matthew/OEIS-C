/*
 * A024432  a(n) = t(1) - t(2) + t(3) + ... + c*t(n), where c = (-1)^(n+1) and
 *          t(j) are Stirling numbers S(n,k) in decreasing order, for k =
 *          1,2,...,n.
 * https://oeis.org/A024432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = t(1) - t(2) + t(3) - … + c·t(n) where t(j) are the
 *            Stirling numbers S(n,k) sorted in decreasing order, and c =
 *            (-1)^(n+1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: comparison for qsort on seq_t elements. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}


/* External linkage */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    /* Two rows of size n + 1 (indices 0 … n). */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    seq_t *cur  = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!prev || !cur) { free(prev); free(cur); return 0; }

    /* Initialise row 1: S(1,1) = 1. */
    for (idx_t i = 0; i <= n; ++i) prev[i] = 0;
    prev[1] = 1;

    /* Build rows 2 … n using the recurrence
       S(i+1,k) = S(i,k‑1) + k·S(i,k). */
    for (idx_t i = 1; i < n; ++i) {
        cur[1] = 1;
        for (idx_t k = 2; k <= i; ++k)
            cur[k] = prev[k - 1] + (seq_t)k * prev[k];
        cur[i + 1] = 1;

        /* Swap pointers so that prev points to the new row. */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;

        /* Clean the old row to avoid stale data (optional). */
        for (idx_t j = 1; j <= i; ++j) cur[j] = 0;
    }
    free(cur);   /* only the second buffer is freed; prev holds the final row */

    /* Sort the Stirling numbers in ascending order. */
    qsort(prev + 1, (size_t)n, sizeof(seq_t), cmp_seq_t);

    /* Alternating sum starting with + on the largest element. */
    seq_t sum  = 0;
    int sign   = 1;                 /* 1 for +, -1 for - */
    for (size_t i = n; i > 0; --i) {
        if (sign > 0)
            sum += prev[i];
        else
            sum -= prev[i];
        sign = -sign;
    }

    free(prev);
    return sum;
}
