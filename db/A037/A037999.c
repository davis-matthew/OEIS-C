/*
 * A037999  Numbers whose maximal base-9 run length is 4.
 * https://oeis.org/A037999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose maximal base‑9 run length is 4
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence enumerates all integers whose base‑9 representation contains
   a run of exactly four equal consecutive digits and no run of five or more.
   The terms are generated in increasing order by increasing the number of
   digits of the base‑9 representation. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache for computed terms */
static seq_t *cache = NULL;         /* pointer to array of seq_t */
static size_t cache_len = 0;        /* number of terms stored */
static size_t cache_cap = 0;        /* current capacity of the cache */
static int next_len = 4;            /* next base‑9 length to generate */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Recursive DFS generating all valid numbers of a given base‑9 length. */
static void dfs_rec(int pos, int len, int last_digit, int run_len,
                    bool has_run4, seq_t val)
{
    if (pos == len) {
        if (has_run4) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        return;
    }

    for (int d = 0; d <= 8; ++d) {
        int new_run = (d == last_digit) ? run_len + 1 : 1;
        if (new_run > 4) continue;              /* reject runs >4 */
        bool new_has = has_run4 || (new_run == 4);
        seq_t next_val = val * (seq_t)9 + (seq_t)d;
        dfs_rec(pos + 1, len, d, new_run, new_has, next_val);
    }
}

/* Generate all valid numbers of base‑9 length 'len'. */
static void generate_len(int len)
{
    for (int first = 1; first <= 8; ++first) {
        dfs_rec(1, len, first, 1, false, (seq_t)first);
    }
}

/* External function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache = seq_alloc(16);      /* initial capacity */
        cache_len = 0;
        cache_cap = 16;
        next_len = 4;
    }

    /* Produce terms until we have at least 'n' of them. */
    while (cache_len < (size_t)n) {
        generate_len(next_len);
        ++next_len;
    }

    return cache[(size_t)n - 1];
}
