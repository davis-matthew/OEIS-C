/*
 * A134436  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n with k cells in the second row (0<=k<=n-1; a deco
 *          polyomino is a directed column-convex polyomino in which the
 *          height, measured along the diagonal, is attained only in the last
 *          column).
 * https://oeis.org/A134436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula for A134436 (triangle read by rows).  
 *
 *  Let R_n(t) = ∑_{k≥1} R_n[k] t^k be the polynomial Q(n-1,1,t) and  
 *      P_n(t) = ∑_{k=0}^{n-1} T(n,k) t^k the generating polynomial of row n.
 *  The recurrences are
 *
 *      R_1(t) = t,
 *      R_n(t) = t[(n-1)! + (n-1) R_{n-1}(t)]          (n ≥ 2)
 *
 *      P_1(t) = 1,
 *      P_n(t) = R_{n-1}(t) + (1 + (n-2)t) P_{n-1}(t)   (n ≥ 2)
 *
 *  The flattened sequence a(1), a(2), … is obtained by writing the rows
 *  in order: a(∑_{i<k} i + j) = T(k,j)   for 0≤j<k.
 *
 *  The implementation builds the triangle row by row, caching already
 *  generated values.  It uses only integer arithmetic on the type seq_t
 *  (which may be 8‑, 16‑, 32‑, 64‑ or 128‑bit, or an arbitrary‑precision
 *  integer).  Memory management is handled with malloc/realloc/free, so
 *  no external utility functions are required.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- cached flattened sequence ---------- */
    static seq_t *cache = NULL;   /* array of all triangle entries */
    static idx_t  cache_len = 0;  /* number of entries stored */
    static idx_t  cache_cap = 0;  /* allocated capacity */

    /* ---------- state of last computed row ---------- */
    static seq_t *P = NULL;        /* P_n(t) coefficients, length curr_n */
    static seq_t *R = NULL;        /* R_n(t) coefficients, length curr_n+1 */
    static seq_t  fact = 0;        /* (curr_n-1)!   for next row */
    static idx_t  curr_n = 0;      /* last row number computed */

    /* ---------- if already cached, return immediately ---------- */
    if (n <= cache_len)
        return cache[n - 1];

    /* ---------- ensure cache capacity for new entries ---------- */
    if (cache_cap < cache_len + n - cache_len) {
        idx_t new_cap = cache_cap ? 2 * cache_cap : 64;
        if (new_cap < cache_len + (n - cache_len))
            new_cap = cache_len + (n - cache_len);
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache) { /* out of memory; return 0 as sentinel */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* ---------- initialise first row if needed ---------- */
    if (curr_n == 0) {
        curr_n = 1;
        /* P_1 = [1] */
        P = (seq_t *)malloc(sizeof(seq_t));
        P[0] = 1;
        /* R_1 = [0,1] */
        R = (seq_t *)malloc(2 * sizeof(seq_t));
        R[0] = 0;
        R[1] = 1;
        /* factorial of (curr_n-1) = 0! = 1 */
        fact = 1;
        /* add first triangle value to cache */
        cache[cache_len++] = 1;
    }

    /* ---------- generate rows until we have at least n entries ---------- */
    while (cache_len < n) {
        /* compute next row number */
        idx_t next_n = curr_n + 1;

        /* ----- update factorial for (next_n-1)! ----- */
        fact = fact * (seq_t)(next_n - 1);

        /* ----- compute R_next(t) from R_prev(t) ----- */
        seq_t *new_R = (seq_t *)malloc((size_t)(next_n + 1) * sizeof(seq_t));
        new_R[0] = 0;
        new_R[1] = fact;
        for (idx_t k = 2; k <= next_n; ++k) {
            new_R[k] = (seq_t)(next_n - 1) * R[k - 1];
        }

        /* ----- compute P_next(t) from R_prev(t) and P_prev(t) ----- */
        seq_t *new_P = (seq_t *)malloc((size_t)next_n * sizeof(seq_t));
        /* coefficient k = 0 */
        new_P[0] = P[0];
        for (idx_t k = 1; k < next_n; ++k) {
            seq_t val = R[k];  /* from R_prev */
            if (k <= next_n - 2)            /* add P_prev[k] if exists */
                val += P[k];
            val += (seq_t)(next_n - 2) * P[k - 1];
            new_P[k] = val;
        }

        /* ----- append new row to cache ----- */
        for (idx_t k = 0; k < next_n; ++k) {
            if (cache_len == cache_cap) {   /* grow cache if full */
                idx_t new_cap = cache_cap ? 2 * cache_cap : 64;
                seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* out of memory; return last computed value as fallback */
                    free(new_P);
                    free(new_R);
                    return 0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = new_P[k];
        }

        /* ----- free previous row arrays and set new ones as current ----- */
        free(P);
        free(R);
        P = new_P;
        R = new_R;
        curr_n = next_n;
    }

    /* ---------- return the requested element ---------- */
    return cache[n - 1];
}
