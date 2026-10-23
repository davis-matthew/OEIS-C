/*
 * A105062  Triangle read by rows, based on the morphism f: 1->2, 2->3, 3->4,
 *          4->5, 5->6, 6->{6,6,10,7}, 7->8, 8->9, 9->10, 10->11, 11->12,
 *          12->{12,12,5,1}. First row is 1. If current row is a,b,c,..., then
 *          the next row is a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A105062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A105062 – triangle read by rows produced by a morphism
 *   f: 1→2, 2→3, 3→4, 4→5, 5→6,
 *      6→{6,6,10,7}, 7→8, 8→9, 9→10,
 *      10→11, 11→12, 12→{12,12,5,1}
 * The sequence is the concatenation of all rows, beginning with row 1=[1].
 * No closed form; the code builds rows until the requested index is reached,
 * memoising the produced terms.
 */

#include <stdlib.h>

/* ensure that *arr contains at least 'needed' elements */
static void ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : *cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* memoised terms (concatenated rows) */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* previous row, used to generate the next */
static seq_t *prev_row = NULL;
static size_t prev_len = 0;   /* length of previous row */

/* one‑time initialisation flag */
static int initialized = 0;

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!initialized) {
        /* first row is [1] */
        prev_len = 1;
        prev_row = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev_row[0] = (seq_t)1;

        /* store the very first term in the cache */
        ensure_capacity(1, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache_size = 1;

        initialized = 1;
    }

    /* quick return if already computed */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* extend the sequence until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* estimate capacity for the next row: max 5× previous length */
        size_t next_cap = prev_len * 5;
        if (next_cap < 1) next_cap = 1;
        seq_t *next_row = (seq_t *)malloc(next_cap * sizeof(seq_t));
        size_t next_len = prev_len;   /* start with copying original row */

        /* copy the original elements first (the row itself) */
        for (size_t i = 0; i < prev_len; ++i)
            next_row[i] = prev_row[i];

        /* append the images under f for each original element */
        for (size_t i = 0; i < prev_len; ++i) {
            seq_t v = prev_row[i];
            if (v == (seq_t)6) {
                next_row[next_len++] = (seq_t)6;
                next_row[next_len++] = (seq_t)6;
                next_row[next_len++] = (seq_t)10;
                next_row[next_len++] = (seq_t)7;
            } else if (v == (seq_t)12) {
                next_row[next_len++] = (seq_t)12;
                next_row[next_len++] = (seq_t)12;
                next_row[next_len++] = (seq_t)5;
                next_row[next_len++] = (seq_t)1;
            } else {
                seq_t mapped;
                if (v == (seq_t)1) mapped = (seq_t)2;
                else if (v == (seq_t)2) mapped = (seq_t)3;
                else if (v == (seq_t)3) mapped = (seq_t)4;
                else if (v == (seq_t)4) mapped = (seq_t)5;
                else if (v == (seq_t)5) mapped = (seq_t)6;
                else if (v == (seq_t)7) mapped = (seq_t)8;
                else if (v == (seq_t)8) mapped = (seq_t)9;
                else if (v == (seq_t)9) mapped = (seq_t)10;
                else if (v == (seq_t)10) mapped = (seq_t)11;
                else if (v == (seq_t)11) mapped = (seq_t)12;
                else mapped = v;          /* safe guard */
                next_row[next_len++] = mapped;
            }
        }

        /* append the new terms to the cache */
        ensure_capacity(cache_size + next_len, &cache, &cache_cap);
        for (size_t i = 0; i < next_len; ++i)
            cache[cache_size++] = next_row[i];

        /* advance to the next row */
        free(prev_row);
        prev_row = next_row;
        prev_len = next_len;
    }

    /* now the cache contains at least n terms */
    return cache[n - 1];
}
