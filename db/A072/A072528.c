/*
 * A072528  Table T(n,k) read by rows, giving number of occurrences of the
 *          remainder k when n is divided by i=1,2,3,...,n.
 * https://oeis.org/A072528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of i with 1 ≤ i ≤ m such that m % i = k-1, where m
 *            is the row containing the n‑th table entry and k = n -
 *            Σ_{t=1}^{m-1} ⌈t/2⌉ is the column index (1‑based)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- maintain cumulative counts Σ⌈t/2⌉ ---------- */
    static idx_t *cum = NULL;
    static size_t sz  = 0;   /* number of stored values */
    static size_t cap = 0;   /* capacity of the array */

    if (!cum) {                     /* first call – initialise */
        cap = 32;
        cum = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cum) return 0;        /* unlikely, but safe */
        cum[0] = 0;                /* Σ⌈t/2⌉ for t = 0 */
        sz = 1;
    }

    /* grow the array until cum[sz-1] >= n */
    while (cum[sz-1] < n) {
        idx_t next_row = (idx_t)sz;               /* next m to compute */
        idx_t add = (next_row + 1) / 2;           /* ceil(next_row/2)   */
        idx_t new_val = cum[sz-1] + add;

        if (sz == cap) {                         /* double the capacity  */
            cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(cum, cap * sizeof(idx_t));
            if (!tmp) return 0;                  /* allocation failed   */
            cum = tmp;
        }
        cum[sz++] = new_val;
    }

    /* ---------- locate the row and column ---------- */
    idx_t m = (idx_t)(sz - 1);               /* row index (m≥1)      */
    idx_t idx_in_row = n - cum[m-1];         /* 1‑based column      */
    idx_t r = idx_in_row - 1;                /* remainder value     */

    /* ---------- count i with remainder r ---------- */
    idx_t cnt = 0;
    for (idx_t i = 1; i <= m; ++i) {
        if (m % i == r) ++cnt;
    }

    return (seq_t)cnt;                       /* fit into seq_t       */
}
