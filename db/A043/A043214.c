/*
 * A043214  Numbers k such that 6 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043214: numbers k such that the base‑9 representation of k contains
   adjacent digits 6 and 8 (either “68” or “86”) but the base‑9
   representation of k‑1 does not.  The routine searches forwards from
   the last candidate and memoises terms so that any n ≥ 1 can be
   requested in arbitrary order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true if base‑9 digits of x contain adjacent 6 and 8 (in either order). */
static bool has_adjacent_68_86(seq_t x)
{
    if (x < 6)               /* cannot have both digits with less than two digits */
        return false;

    seq_t prev = x % 9;
    x /= 9;
    while (x) {
        seq_t cur = x % 9;
        if ((cur == 6 && prev == 8) || (cur == 8 && prev == 6))
            return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

/* Memoisation cache */
static seq_t *cache      = NULL;   /* stored terms                 */
static size_t cache_len  = 0;      /* number of terms stored       */
static size_t cache_cap  = 0;      /* allocated capacity           */
static seq_t next_k      = 1;      /* next number to test          */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *p = seq_realloc(cache, cache_cap, new_cap);
    if (!p)
        abort();

    cache   = p;
    cache_cap = new_cap;
}

/* Required function. */
seq_t a(idx_t n)
{
    /* Build terms up to the requested index. */
    while (cache_len < (size_t)n) {
        if (has_adjacent_68_86(next_k) && !has_adjacent_68_86(next_k - 1)) {
            ensure_cache(cache_len + 1);
            cache[cache_len] = next_k;
            ++cache_len;
        }
        ++next_k;
    }
    return cache[n - 1];
}
