/*
 * A060923  Bisection of Lucas triangle A060922: even-indexed members of
 *          column sequences of A060922 (not counting leading zeros).
 * https://oeis.org/A060923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = value at index n in the flattened Lucas triangle *
 *            (row‑wise enumeration). The triangle is defined by * T(0,0)=1,
 *            T(1,0)=4, T(1,1)=1, * T(2,0)=11, T(2,1)=17, T(2,2)=1, * and for
 *            n≥3 * T(n,k) = 3 T(n‑1,k)+2 T(n‑1,k‑1) * + 2 T(n‑2,k‑1)+4
 *            T(n‑3,k‑2) * – T(n‑2,k) – T(n‑2,k‑2), * with T(n,k)=0...
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of all computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static storage for the last three rows of T */
    static seq_t *row[3] = {NULL, NULL, NULL};
    static size_t row_len[3] = {0, 0, 0};
    static idx_t last_row = (idx_t)-1; /* no rows computed yet */

    /* initialise on first call */
    if (cache == NULL) {
        /* allocate cache buffer */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, fall back to 0 */

        /* base rows --------------------------------------------------- */
        /* row 0 : {1} */
        row[0] = (seq_t *)malloc(1 * sizeof(seq_t));
        row[0][0] = (seq_t)1;
        row_len[0] = 1;
        /* row 1 : {4,1} */
        row[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        row[1][0] = (seq_t)4;
        row[1][1] = (seq_t)1;
        row_len[1] = 2;
        /* row 2 : {11,17,1} */
        row[2] = (seq_t *)malloc(3 * sizeof(seq_t));
        row[2][0] = (seq_t)11;
        row[2][1] = (seq_t)17;
        row[2][2] = (seq_t)1;
        row_len[2] = 3;
        last_row = 2;

        /* fill cache with the first three rows */
        for (size_t i = 0; i < row_len[0]; ++i) cache[cache_len++] = row[0][i];
        for (size_t i = 0; i < row_len[1]; ++i) cache[cache_len++] = row[1][i];
        for (size_t i = 0; i < row_len[2]; ++i) cache[cache_len++] = row[2][i];
    }

    /* grow cache if needed */
#define ENSURE_CACHE_CAP(new_cap)                                  \
    do {                                                           \
        if (new_cap > cache_cap) {                                 \
            size_t newc = cache_cap;                               \
            while (newc < new_cap) newc *= 2;                      \
            seq_t *tmp = (seq_t *)realloc(cache, newc * sizeof(seq_t)); \
            if (!tmp) return (seq_t)0; /* allocation failure */   \
            cache = tmp; cache_cap = newc;                         \
        }                                                          \
    } while (0)

    /* generate rows until cache has index n */
    while (cache_len <= (size_t)n) {
        idx_t next_row = last_row + 1;      /* next row to compute */
        size_t len = (size_t)(next_row + 1);
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur) return (seq_t)0;          /* allocation failure */

        /* obtain previous rows if available */
        seq_t *prev1 = (next_row >= 1) ? row[(next_row-1)%3] : NULL;
        seq_t *prev2 = (next_row >= 2) ? row[(next_row-2)%3] : NULL;
        seq_t *prev3 = (next_row >= 3) ? row[(next_row-3)%3] : NULL;

        for (size_t k = 0; k < len; ++k) {
            /* accumulate positive and negative contributions separately */
            seq_t pos = 0, neg = 0;

            /* 3*T(n-1,k) + 2*T(n-1,k-1) */
            if (prev1) {
                if (k < row_len[(next_row-1)%3])
                    pos += (seq_t)3 * prev1[k];
                if (k > 0 && k-1 < row_len[(next_row-1)%3])
                    pos += (seq_t)2 * prev1[k-1];
            }

            /* -T(n-2,k) + 2*T(n-2,k-1) - T(n-2,k-2) */
            if (prev2) {
                if (k < row_len[(next_row-2)%3])
                    neg += prev2[k];
                if (k > 0 && k-1 < row_len[(next_row-2)%3])
                    pos += (seq_t)2 * prev2[k-1];
                if (k > 1 && k-2 < row_len[(next_row-2)%3])
                    neg += prev2[k-2];
            }

            /* +4*T(n-3,k-2) */
            if (prev3 && k > 1 && k-2 < row_len[(next_row-3)%3])
                pos += (seq_t)4 * prev3[k-2];

            cur[k] = pos - neg;
        }

        /* append row to cache */
        ENSURE_CACHE_CAP(cache_len + len);
        for (size_t i = 0; i < len; ++i)
            cache[cache_len++] = cur[i];

        /* replace old row in the ring buffer */
        int idx = (int)(next_row % 3);
        if (row[idx]) free(row[idx]);
        row[idx] = cur;
        row_len[idx] = len;

        last_row = next_row;
    }

    return cache[(size_t)n];
}
