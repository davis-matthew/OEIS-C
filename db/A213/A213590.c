/*
 * A213590  Rectangular array: (row n) = b**c, where b(h) = h^2, c(h) =
 *          F(n-1+h), F = A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 153, u32 n <= 820, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213590 – Antidiagonal read of a rectangular array
 *
 *  The array T(n,k) (n ≥ 1, k ≥ 1) is defined by
 *
 *      • T(1,1); …; T(1,5) = 1, 5, 15, 36, 76
 *      • T(2,1); …; T(2,5) = 1, 6, 20, 51, 112
 *      • For n = 1 or 2 and k > 5:
 *            T(n,k) = 4·T(n,k‑1) – 5·T(n,k‑2) + T(n,k‑3)
 *                   + 2·T(n,k‑4) – T(n,k‑5)
 *      • For n ≥ 3:
 *            T(n,k) = T(n‑1,k) + T(n‑2,k)      (column recurrence)
 *
 *  The sequence a(n) is the array read by falling antidiagonals:
 *      a(1) = T(1,1),  a(2) = T(1,2),  a(3) = T(2,1),  a(4) = T(1,3), …
 *
 *  The implementation below caches computed rows/columns of T so that
 *  each (n,k) is produced once.
 */

#include <stdint.h>
#include <stdlib.h>

/* local helpers for memory allocation that are safe in both C and C++ */
static seq_t *alloc_seq(size_t n) {
#if defined(__cplusplus)
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq(seq_t *p) {
#if defined(__cplusplus)
    delete [] p;
#else
    free(p);
#endif
}

/* static cache for computed table */
static seq_t *cache   = NULL;
static idx_t cache_rows = 0;
static idx_t cache_cols = 0;

/* first five columns of the first two rows */
static const seq_t row1_init[5] = {1, 5, 15, 36, 76};
static const seq_t row2_init[5] = {1, 6, 20, 51, 112};

/* recompute the cache if larger dimensions are required */
static void recompute_cache(idx_t new_rows, idx_t new_cols)
{
    seq_t *new_cache = alloc_seq(new_rows * new_cols);
    if (!new_cache) exit(1);   /* abort on allocation failure */

    /* ---------- row 1 ---------- */
    for (idx_t c = 1; c <= new_cols; ++c) {
        seq_t val;
        if (c <= 5) {
            val = row1_init[c - 1];
        } else {
            seq_t a1 = new_cache[0 * new_cols + (c - 2)];
            seq_t a2 = new_cache[0 * new_cols + (c - 3)];
            seq_t a3 = new_cache[0 * new_cols + (c - 4)];
            seq_t a4 = new_cache[0 * new_cols + (c - 5)];
            seq_t a5 = new_cache[0 * new_cols + (c - 6)];
            val = 4 * a1 - 5 * a2 + a3 + 2 * a4 - a5;
        }
        new_cache[0 * new_cols + (c - 1)] = val;
    }

    /* ---------- row 2 ---------- */
    if (new_rows >= 2) {
        for (idx_t c = 1; c <= new_cols; ++c) {
            seq_t val;
            if (c <= 5) {
                val = row2_init[c - 1];
            } else {
                seq_t a1 = new_cache[1 * new_cols + (c - 2)];
                seq_t a2 = new_cache[1 * new_cols + (c - 3)];
                seq_t a3 = new_cache[1 * new_cols + (c - 4)];
                seq_t a4 = new_cache[1 * new_cols + (c - 5)];
                seq_t a5 = new_cache[1 * new_cols + (c - 6)];
                val = 4 * a1 - 5 * a2 + a3 + 2 * a4 - a5;
            }
            new_cache[1 * new_cols + (c - 1)] = val;
        }
    }

    /* ---------- rows n ≥ 3 using the column recurrence ---------- */
    for (idx_t r = 3; r <= new_rows; ++r) {
        for (idx_t c = 1; c <= new_cols; ++c) {
            new_cache[(r - 1) * new_cols + (c - 1)] =
                new_cache[(r - 2) * new_cols + (c - 1)] +
                new_cache[(r - 3) * new_cols + (c - 1)];
        }
    }

    free_seq(cache);
    cache      = new_cache;
    cache_rows = new_rows;
    cache_cols = new_cols;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find the antidiagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    idx_t prev_sum = (d - 1) * d / 2;
    idx_t pos      = n - prev_sum - 1;   /* zero‑based position on diagonal d */
    idx_t row      = pos + 1;            /* row index (starting at 1) */
    idx_t col      = d - row + 1;        /* column index (starting at 1) */

    /* enlarge the cache if necessary */
    if (row > cache_rows || col > cache_cols)
        recompute_cache(row > cache_rows ? row : cache_rows,
                        col > cache_cols ? col : cache_cols);

    return cache[(row - 1) * cache_cols + (col - 1)];
}
