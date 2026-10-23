/*
 * A171077  Numbers that are multiples of 28 and contain both a 4 and a 7.
 * https://oeis.org/A171077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth multiple of 28 that contains both digits 4 and 7 */
/* (OEIS A171077) */
#include <stddef.h>
#include <stdbool.h>
#if defined(__cplusplus)
#include <new>
#endif

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_cap = 0;     /* current capacity of the array */
static size_t cache_len = 0;     /* number of terms stored so far */
static idx_t last_k = 1;         /* next multiplier of 28 to test */

/* Helper: grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap) {
#if defined(__cplusplus)
    seq_t *newp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) newp[i] = cache[i];
        delete[] cache;
    }
    cache = newp;
    cache_cap = new_cap;
#else
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
#endif
}

/* Helper: test if a number contains both digits 4 and 7. */
static bool has4and7(seq_t x) {
    bool has4 = false, has7 = false;
    if (x == 0) return false;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 4) has4 = true;
        else if (d == 7) has7 = true;
        if (has4 && has7) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1, but safe fallback */

    /* Ensure the cache has enough terms for index n. */
    while (cache_len < (size_t)n) {
        /* Find the next multiple of 28 that contains both 4 and 7. */
        for (;;) {
            seq_t candidate = (seq_t)28 * (seq_t)last_k;
            if (has4and7(candidate)) {
                if (cache_len == cache_cap)
                    grow_cache((cache_cap ? cache_cap * 2 : 16));
                cache[cache_len++] = candidate;
                ++last_k;  /* move to the next multiplier for subsequent searches */
                break;
            }
            ++last_k;
        }
    }
    return cache[n - 1];
}
