/*
 * A135737  Ulam type (1-additive) sequences u[1]=2, u[2]=2n+1, u[k+1] is
 *          least unique sum u[i]+u[j]>u[k], 1<=i<j<=k; formatted as a table
 *          and read by descending antidiagonals.
 * https://oeis.org/A135737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   allocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135737 (Ulam type 1-additive) */
/* a(n) = U(2,2*row+1)[col] where (row, col) are derived from n by
 * reading the table T[n,k] = U(2,2n+1)[k] in antidiagonal order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allocate_seq_array.h"

/* Compare two seq_t values for qsort */
static int cmp_seq(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Allocate an array of seq_t of given size */

/* Free an array allocated by allocate_seq_array */
static void free_seq_array(seq_t *arr) {
#ifdef __cplusplus
    delete [] arr;
#else
    free(arr);
#endif
}

/* Compute the k-th element of U(2,2*row+1).  row and k are 1‑based. */
static seq_t ulam1_additive(idx_t row, idx_t k) {
    if (k == 1) return (seq_t)2;
    if (k == 2) return (seq_t)(2 * (seq_t)row + 1);

    seq_t *u = allocate_seq_array(k);
    if (!u) return 0;                       /* allocation failure */
    u[0] = (seq_t)2;
    u[1] = (seq_t)(2 * (seq_t)row + 1);

    idx_t len = 2;                          /* current length */
    while (len < k) {
        seq_t last = u[len-1];
        idx_t pair_cnt = len * (len-1) / 2;
        seq_t *sums = allocate_seq_array(pair_cnt);
        if (!sums) { free_seq_array(u); return 0; }

        /* build all pair sums */
        idx_t idx = 0;
        for (idx_t i = 0; i < len; ++i)
            for (idx_t j = i + 1; j < len; ++j)
                sums[idx++] = u[i] + u[j];

        /* sort to detect duplicates */
        qsort(sums, pair_cnt, sizeof(seq_t), cmp_seq);

        /* find smallest sum > last that occurs exactly once */
        seq_t next = last;
        idx_t i = 0;
        while (i < pair_cnt) {
            seq_t val = sums[i];
            if (val <= last) { ++i; continue; }
            idx_t j = i + 1;
            while (j < pair_cnt && sums[j] == val) ++j;
            if (j - i == 1) { next = val; break; }
            i = j;
        }

        free_seq_array(sums);
        u[len] = next;
        ++len;
    }

    seq_t result = u[k-1];
    free_seq_array(u);
    return result;
}

/* Main function required by the evaluation harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* offset is 1, but handle 0 */

    /* Determine antidiagonal 's' such that s*(s+1)/2 >= n */
    idx_t s = 1;
    while (((unsigned __int128)s * (s + 1)) / 2 < n) ++s;

    /* Indices on this antidiagonal */
    idx_t prev = (s - 1) * s / 2;           /* last index of previous antidiagonal */
    idx_t pos  = n - prev;                  /* 1‑based position on antidiagonal */
    idx_t row  = pos;                       /* row index in the table T */
    idx_t col  = s - pos + 1;               /* column index in the table T */

    return ulam1_additive(row, col);
}
