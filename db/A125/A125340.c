/*
 * A125340  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A125340: Number of base‑30 circular n‑digit numbers with
   adjacent digits differing by at most 3.
   Formula: a(n) = trace(A^n), where A is the 30×30 adjacency matrix
   A[i][j] = 1 if |i–j| ≤ 3 else 0.  a(0) = 1. */
#include <stdlib.h>

/* Count of circular n‑digit base‑30 numbers with adjacent digits
   differing by ≤ 3, for arbitrary precision integer type seq_t. */
seq_t a(idx_t n)
{
    /* ---------- static state ---------------------------------------- */
    static seq_t *cache = NULL;          /* cache[0..max_n]  */
    static size_t cache_capacity = 0;    /* allocated elements */
    static idx_t  max_n = 0;             /* highest n already computed   */
    static seq_t *mat_cur  = NULL;       /* current matrix A^max_n       */
    static seq_t *mat_next = NULL;       /* scratch for next multiplication */

    /* ---------- initialise cache for n = 0 if necessary ------------- */
    if (cache == NULL) {
        cache_capacity = 1;                 /* just a[0] */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        max_n   = 0;
    }

    /* ---------- grow cache array if n is larger --------------------- */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache,
                                     sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* ---------- return directly if already known -------------------- */
    if (n <= max_n) {
        return cache[n];
    }

    /* ---------- initialise matrices if first time we need n ≥ 1 ----- */
    if (max_n == 0) {
        /* build adjacency matrix A (30×30) */
        mat_cur  = (seq_t*)malloc(sizeof(seq_t) * 900); /* 30*30 */
        mat_next = (seq_t*)malloc(sizeof(seq_t) * 900);
        if (!mat_cur || !mat_next)
            return (seq_t)0;                  /* allocation failed */

        for (int i = 0; i < 30; ++i) {
            for (int j = 0; j < 30; ++j) {
                int d = i - j;
                mat_cur[i*30 + j] =
                    (d < 0 ? -d : d) <= 3 ? (seq_t)1 : (seq_t)0;
            }
        }
        /* trace of A is 30 */
        seq_t trace = (seq_t)0;
        for (int i = 0; i < 30; ++i) trace += mat_cur[i*30 + i];
        cache[1] = trace;
        max_n = 1;
    }

    /* ---------- compute powers up to n -------------------------------- */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* mat_next = mat_cur * A, using the sparsity of A */
        for (int i = 0; i < 30; ++i) {
            /* prefix sums of row i of mat_cur */
            seq_t pref[31];
            pref[0] = (seq_t)0;
            size_t base = (size_t)i * 30;
            for (int j = 0; j < 30; ++j) {
                pref[j + 1] = pref[j] + mat_cur[base + j];
            }
            /* compute new row */
            for (int j = 0; j < 30; ++j) {
                int l = j - 3; if (l < 0) l = 0;
                int r = j + 3; if (r > 29) r = 29;
                mat_next[base + j] = pref[r + 1] - pref[l];
            }
        }
        /* compute trace of mat_next and store result */
        seq_t trace = (seq_t)0;
        for (int i = 0; i < 30; ++i) trace += mat_next[i*30 + i];
        cache[k] = trace;

        /* swap references for next iteration */
        seq_t *tmp = mat_cur;
        mat_cur = mat_next;
        mat_next = tmp;
    }
    max_n = n;

    return cache[n];
}
