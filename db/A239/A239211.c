/*
 * A239211  Squares that are not divisible by any of their nonzero digits.
 * https://oeis.org/A239211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 922 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) from OEIS A239211.
   a(n) = the nth square whose decimal representation contains no
          non‑zero digit that divides the square itself.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

/* Helper: test whether m^2 satisfies the digit divisibility
   condition.  m is an idx_t, square is a seq_t. */
static bool square_ok(idx_t m)
{
    seq_t sq = (seq_t)m * (seq_t)m;      /* m^2 */
    seq_t tmp = sq;
    while (tmp != (seq_t)0) {
        seq_t d = tmp % (seq_t)10;       /* extract a decimal digit */
        if (d != (seq_t)0) {
            if (sq % d == (seq_t)0)      /* divisible by a non‑zero digit */
                return false;
        }
        tmp /= (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* out of range, may be ignored */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of stored terms */
    static size_t cache_cap = 0;          /* allocated capacity */
    static idx_t last_m = 0;              /* largest m tested so far */

    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        ++last_m;
        if (square_ok(last_m)) {
            /* Ensure enough space */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();     /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_m * (seq_t)last_m;
        }
    }
    return cache[n - 1];
}
