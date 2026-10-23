/*
 * A014630  Distinct elements occurring in triangle of Eulerian numbers
 *          (unsorted).
 * https://oeis.org/A014630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = the nth distinct Eulerian number encountered * when rows
 *            of the Eulerian triangle are listed from row 1 upward. * a(1)=1,
 *            a(2)=4, a(3)=11, … (as given in OEIS A014630)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* -------------- cache of distinct values -------------------- */
static seq_t *cache      = NULL;   /* array of distinct Eulerian numbers */
static size_t cache_len  = 0;      /* current number of stored values   */
static size_t cache_cap  = 0;      /* capacity of the array             */

/* append a new distinct value to the cache */
static void append_value(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();                  /* out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }
    cache[cache_len++] = v;
}

/* check whether v has already been seen */
static bool in_cache(seq_t v)
{
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i] == v)
            return true;
    return false;
}

/* -------------- state for generating Eulerian rows -------- */
static seq_t *prev_row   = NULL;   /* Eulerian numbers of the previous row */
static size_t prev_len   = 0;      /* length of prev_row (row number)     */
static idx_t  last_row   = 0;      /* last processed row number (0 if none) */

/* ------------------------------------------------------------------
 * a(n): return the n-th distinct Eulerian number (1‑based).
 * The function generates rows of the Eulerian triangle on demand
 * and keeps an ordered list of distinct values.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* generate rows until we have at least n distinct values */
    while (cache_len < (size_t)n) {
        idx_t row;          /* number of the current row */
        size_t cur_len;     /* length of current row */
        seq_t *cur_row;     /* array for current row */

        /* ----- first row (n = 1) -------------------------------- */
        if (last_row == 0) {
            row     = 1;
            cur_len = 1;
            cur_row = (seq_t *)malloc(sizeof(seq_t) * cur_len);
            if (!cur_row) abort();
            cur_row[0] = (seq_t)1;          /* E(1,0) = 1 */

            if (!in_cache(cur_row[0]))
                append_value(cur_row[0]);

            /* initialise state */
            prev_row = cur_row;
            prev_len = cur_len;
            last_row = row;
            continue;                       /* next iteration */
        }

        /* ----- generate row (row = last_row + 1) ----------------- */
        row     = last_row + 1;
        cur_len = (size_t)row;
        cur_row = (seq_t *)malloc(sizeof(seq_t) * cur_len);
        if (!cur_row) abort();

        cur_row[0]                = (seq_t)1;
        cur_row[cur_len - 1]      = (seq_t)1;

        /* inner elements:  E(n,k) = (n-k) * E(n-1,k-1) + (k+1) * E(n-1,k) */
        for (size_t k = 1; k + 1 < cur_len; ++k) {
            seq_t mult1 = (seq_t)(row - (idx_t)k);
            seq_t mult2 = (seq_t)(k + 1);
            seq_t part1 = prev_row[k - 1] * mult1;
            seq_t part2 = prev_row[k]     * mult2;
            cur_row[k] = part1 + part2;
        }

        /* record first occurrences in order of appearance */
        for (size_t k = 0; k < cur_len; ++k) {
            if (!in_cache(cur_row[k]))
                append_value(cur_row[k]);
        }

        /* free previous row, update state for next iteration */
        free(prev_row);
        prev_row = cur_row;
        prev_len = cur_len;
        last_row = row;
    }

    return cache[(size_t)n - 1];
}
