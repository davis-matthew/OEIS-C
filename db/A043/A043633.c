/*
 * A043633  Numbers whose base-9 representation has exactly 4 runs.
 * https://oeis.org/A043633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are the n-th integers whose base‑9 representation
   contains exactly four runs (maximal blocks of equal digits). */
/* This implementation enumerates numbers one by one, counting
   runs in base 9, and stores results in a dynamically grown
   array to answer any query efficiently. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array of found numbers   */
static idx_t  cache_len = 0;     /* current number of entries in cache */
static idx_t  cache_cap = 0;     /* allocated capacity of cache     */
static seq_t  last_checked = 0;  /* last integer examined in enumeration */

/* Count the number of runs in the base‑9 representation of x.
   Runs are counted from the least significant digit upwards;
   this gives the same run count as the usual left‑to‑right order. */
static inline int count_base9_runs(seq_t x)
{
    int runs = 0;
    int prev = -1;            /* previous digit (none at start)   */
    while (x > 0)
    {
        int digit = (int)(x % 9);
        if (digit != prev)
        {
            ++runs;
            prev = digit;
        }
        x /= 9;
        if (runs > 4)          /* early exit if more than 4 runs */
            return runs;
    }
    return runs;
}

/* Extend the cache until it contains at least n entries. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        seq_t candidate = last_checked + 1;
        /* Count runs for the candidate number. */
        if (count_base9_runs(candidate) == 4)
        {
            if (cache_len == cache_cap)
            {
                /* Grow the cache (initial capacity 128). */
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        last_checked = candidate;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 – undefined for n=0 */
    ensure_cache(n);
    return cache[n - 1];
}
