/*
 * A037370  Numbers whose base-8 and base-9 expansions have no digits in
 *          common.
 * https://oeis.org/A037370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A037370:
   numbers whose base‑8 and base‑9 expansions have no digits in common.
   The function generates terms on demand using a linear search and caches
   results so that calls with arbitrary indices are efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_is_good.h"

static seq_t *_a_cache = NULL;    /* cache of computed terms (0‑based)   */
static size_t _a_cache_len = 0;   /* length of cache                     */
static size_t _a_cache_cap = 0;   /* current capacity of cache           */
static seq_t _last_checked = 0;   /* last integer examined (not last good) */

/* Grow the cache to hold at least `need` entries. */
static void _ensure_cache_capacity(size_t need)
{
    if (need <= _a_cache_cap)
        return;
    size_t new_cap = _a_cache_cap ? _a_cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(_a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    _a_cache = tmp;
    _a_cache_cap = new_cap;
}

/* Append a new good number to the cache. */
static void _append_cache(seq_t val)
{
    _ensure_cache_capacity(_a_cache_len + 1);
    _a_cache[_a_cache_len++] = val;
}

/* Compute the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n<1, but return 0 for safety */
        return (seq_t)0;
    while (_a_cache_len < (size_t)n) {
        _last_checked++;
        if (_is_good(_last_checked))
            _append_cache(_last_checked);
    }
    return _a_cache[n - 1];
}
