/*
 * A090737  Triangle of Eulerian numbers modulo 3.
 * https://oeis.org/A090737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Triangle of Eulerian numbers modulo 3, flattened row‑wise.
 * a(n) is the n-th element (1‑indexed) of the infinite array:
 *    row 1: 1
 *    row 2: 1 1
 *    row 3: 1 4 1  (mod 3 → 1 1 1)
 *    row 4: 1 11 11 1  (mod 3 → 1 2 2 1)
 *    …
 * The values are computed by the standard recurrence
 *
 *   A(n,0) = A(n,n‑1) = 1
 *   A(n,k) = (n‑k)·A(n‑1,k‑1) + (k+1)·A(n‑1,k)   for 0 < k < n‑1
 *
 * All arithmetic is performed modulo 3.
 */

#include <stdlib.h>
#include <stdint.h>

/* Find the row (m) that contains the flattened index n.
 * Row m has length m and the total number of elements up to row m is
 * the triangular number T(m) = m·(m+1)/2.  m is the smallest integer
 * with T(m) ≥ n. */
static inline size_t row_from_n(idx_t n)
{
    size_t low  = 1;
    size_t high = 1;
    /* increase high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* binary search */
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    static seq_t **rows      = NULL; /* rows[1 .. m] */
    static size_t rows_cap   = 0;
    static size_t rows_used  = 0;

    /* locate the row m and the column k (0‑based) of the n‑th
     * flattened element */
    size_t m = row_from_n(n);
    idx_t  prev_T = (idx_t)((unsigned __int128)(m - 1) * m / 2);
    size_t k = (size_t)(n - prev_T - 1);

    /* grow the row pointer array if necessary */
    if (rows_used < m) {
        if (rows_cap < m) {
            size_t new_cap = rows_cap ? rows_cap << 1 : 1;
            while (new_cap < m)
                new_cap <<= 1;
            seq_t **tmp = (seq_t **)realloc(rows,
                                            (new_cap + 1) * sizeof(seq_t *));
            /* initialise new slots to NULL (not strictly needed) */
            for (size_t i = rows_used + 1; i <= new_cap; ++i)
                tmp[i] = NULL;
            rows     = tmp;
            rows_cap = new_cap;
        }

        /* generate rows incrementally up to row m */
        for (size_t i = rows_used + 1; i <= m; ++i) {
            size_t len = i;                       /* row length */
            seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
            cur[0] = 1;                            /* A(i,0) = 1 */
            if (len > 1)
                cur[len - 1] = 1;                  /* A(i,i‑1) = 1 */

            if (i >= 3) {
                seq_t *prev = rows[i - 1];
                for (size_t j = 1; j + 1 < i; ++j) {
                    seq_t t1 = (seq_t)(i - j) * prev[j - 1];
                    seq_t t2 = (seq_t)(j + 1) * prev[j];
                    cur[j] = (t1 + t2) % 3;          /* modulo 3 */
                }
            }
            rows[i] = cur;
        }
        rows_used = m;
    }

    return rows[m][k];
}
