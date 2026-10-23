/*
 * A029598  Numbers represented by quadratic form with Gram matrix [ 2, 1, 0;
 *          1, 2, 1; 0, 1, 3 ].
 * https://oeis.org/A029598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029598: Numbers represented by the quadratic form
 *  Q(x,y,z) = 2x² + 2xy + 2y² + 2yz + 3z²      (Gram matrix [[2,1,0],[1,2,1],[0,1,3]])
 *  a(n) is the n-th non‑negative integer in the sorted list of values of Q over all integer triples (x,y,z).
 *  The routine builds a cache of representable values and extends it as required for larger n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparison function for qsort on seq_t values */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t a = *(const seq_t *)p;
    const seq_t b = *(const seq_t *)q;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Allocate and fill the cache of representable values for given M
 * (search range -M .. M for each coordinate).  The cache is sorted
 * and deduplicated.  The caller passes the address of cache_ptr and
 * cache_len to receive the new data.  If memory allocation fails,
 * the program aborts. */
static void build_cache(idx_t requested_n, seq_t **cache_ptr,
                        size_t *cache_len, idx_t *max_coord)
{
    /* Start with an initial search range that yields enough values.
       If requested_n exceeds the number of unique values generated,
       increase the range until enough values are stored. */
    idx_t M = *max_coord;
    if (M < 50) M = 50;  /* minimum range to capture small values like 10 */
    size_t prev_len = 0;
    seq_t *prev_cache = NULL;

    while (true) {
        /* total number of triples in current search space */
        size_t total = (size_t)(2 * M + 1) * (2 * M + 1) * (2 * M + 1);
        seq_t *vals = (seq_t *)malloc(total * sizeof(seq_t));
        if (!vals) abort();

        size_t cnt = 0;
        for (int x = -(int)M; x <= (int)M; ++x) {
            seq_t sx = (seq_t)x;
            for (int y = -(int)M; y <= (int)M; ++y) {
                seq_t sy = (seq_t)y;
                /* Pre‑compute terms that depend only on x,y to save work */
                seq_t term_xy_y = (seq_t)2 * sx * sy;         /* 2xy */
                seq_t term_xy2  = (seq_t)2 * sy * sy;         /* 2y² */
                seq_t term_xx   = (seq_t)2 * sx * sx;         /* 2x² */
                for (int z = -(int)M; z <= (int)M; ++z) {
                    seq_t sz = (seq_t)z;
                    seq_t v = term_xx
                        + term_xy_y
                        + term_xy2
                        + (seq_t)2 * sy * sz       /* 2yz */
                        + (seq_t)3 * sz * sz;      /* 3z² */
                    vals[cnt++] = v;
                }
            }
        }

        /* Sort all produced values */
        qsort(vals, cnt, sizeof(seq_t), cmp_seq_t);

        /* Count unique values */
        size_t uniq = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (i == 0 || vals[i] != vals[i - 1]) {
                ++uniq;
            }
        }

        if (uniq >= requested_n) {
            /* Allocate final cache with uniq entries */
            seq_t *final_cache = (seq_t *)malloc(uniq * sizeof(seq_t));
            if (!final_cache) abort();
            size_t j = 0;
            for (size_t i = 0; i < cnt; ++i) {
                if (i == 0 || vals[i] != vals[i - 1]) {
                    final_cache[j++] = vals[i];
                }
            }
            free(vals);
            /* Release previous cache if any */
            if (prev_cache) free(prev_cache);

            *cache_ptr = final_cache;
            *cache_len = uniq;
            *max_coord = M;
            return;
        }

        /* Not enough unique values yet; enlarge search space and retry. */
        free(vals);
        if (prev_cache) free(prev_cache);
        prev_cache = NULL;
        prev_len = 0;
        M *= 2;   /* double the range */
    }
}

/* Global static cache state */
static seq_t *global_cache = NULL;
static size_t global_cache_len = 0;
static idx_t global_max_coord = 0;

/* Main function to return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 1, return 0 as fallback */

    /* Build or extend cache until it contains at least n elements */
    if (global_cache == NULL || global_cache_len < n) {
        build_cache(n, &global_cache, &global_cache_len, &global_max_coord);
    }

    /* Return the (n-1)-th element (0‑based indexing) */
    return global_cache[n - 1];
}
