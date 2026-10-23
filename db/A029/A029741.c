/*
 * A029741  Even numbers with distinct digits.
 * https://oeis.org/A029741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   a(n) is the n‑th even number in ascending order whose decimal
 *            digits are all distinct (OEIS A029741)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* ------------------------------------------------------------------ */
/* Helper to grow a dynamic array of seq_t values                        */
/* ------------------------------------------------------------------ */
static void append_seq(seq_t **array, size_t *cap, size_t *len, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 128 : (*cap * 2);
        seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *array = tmp;
        *cap   = new_cap;
    }
    (*array)[*len] = val;
    (*len)++;
}

/* ------------------------------------------------------------------ */
/* Depth‑first search that enumerates all even numbers with distinct
   digits of a fixed length in ascending numeric order.                */
/* ------------------------------------------------------------------ */
static void dfs(int len, int pos, seq_t cur, bool used[10],
                seq_t **array, size_t *cap, size_t *len_out)
{
    if (pos == len) {
        append_seq(array, cap, len_out, cur);
        return;
    }

    int start = 0;
    if (pos == 0 && len > 1) start = 1;          /* no leading zero for multi‑digit numbers */

    for (int d = start; d <= 9; ++d) {
        if (used[d]) continue;
        if (pos == len - 1 && (d & 1)) continue; /* last digit must be even */
        used[d] = true;
        dfs(len, pos + 1, cur * 10 + (seq_t)d, used,
            array, cap, len_out);
        used[d] = false;
    }
}

/* ------------------------------------------------------------------ */
/* Build the entire sequence once and cache it for future look‑ups.   */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void init_cache(void)
{
    if (cache != NULL) return;   /* already built */

    cache_cap = 128;                     /* initial capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    bool used[10] = {0};
    for (int len = 1; len <= 10; ++len) {
        dfs(len, 0, 0, used, &cache, &cache_cap, &cache_len);
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_cache();

    /* offset 1: a(1) is the first term in the cache */
    if (n == 0) return 0;   /* undefined, but return 0 for safety */

    size_t idx = (size_t)((idx_t)n - 1);
    assert(idx < cache_len);      /* n must be within the known range */

    return cache[idx];
}
