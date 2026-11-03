/*
 * A039261  Numbers whose base-12 representation has the same number of 6's
 *          and 8's.
 * https://oeis.org/A039261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 975 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑12 representation has the same number of 6's and 8's
   (computed by brute‑force search of all integers). */

#include <stddef.h>
#include <stdbool.h>
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

static bool has_equal_68(seq_t x)
{
    unsigned count6 = 0, count8 = 0;
    if (x == (seq_t)0) return true;          /* 0 has no digits → 0==0 */
    while (x > (seq_t)0) {
        seq_t digit = x % (seq_t)12;
        if (digit == (seq_t)6)   count6++;
        else if (digit == (seq_t)8) count8++;
        x /= (seq_t)12;
    }
    return count6 == count8;
}

seq_t a(idx_t n)
{
    /* Static cache holding all terms found so far. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = (seq_t)0;

    /* Initialize the cache with the first term (0). */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Extend the cache up to the required index n. */
    while (cache_len < (size_t)n) {
        seq_t x = last_checked + (seq_t)1;
        while (true) {
            if (has_equal_68(x)) {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) return (seq_t)0; /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = x;
                last_checked = x;
                break;
            }
            x = x + (seq_t)1;
        }
    }
    return cache[n - 1];
}
