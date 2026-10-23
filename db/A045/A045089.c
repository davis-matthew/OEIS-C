/*
 * A045089  Numbers whose base-4 representation contains no 1's and exactly
 *          four 2's.
 * https://oeis.org/A045089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate numbers whose base‑4 representation contains
   no 1's and exactly four 2's, sorted in increasing order. */

#include <stdbool.h>
#include <stdlib.h>

/* convert a candidate to base‑4 and check the condition */
static bool valid_base4(seq_t x) {
    unsigned cnt2 = 0;          /* Number of 2's found */
    if (x == (seq_t)0) return false;

    while (x != (seq_t)0) {
        seq_t digit = x % (seq_t)4;
        if (digit == (seq_t)1) return false;   /* forbidden digit */
        if (digit == (seq_t)2) ++cnt2;
        x /= (seq_t)4;
    }
    return cnt2 == 4;
}

/* dynamic cache of computed terms */
static seq_t *cache      = NULL;  /* array of terms */
static size_t cache_size = 0;     /* allocated capacity */
static idx_t  cache_count = 0;    /* number of terms currently stored */
static seq_t  next_candidate = (seq_t)1;  /* integer to test next */

/* grow the cache when needed */
static void grow_cache(void) {
    if (cache_size == 0) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);   /* out of memory */
    } else if (cache_count == cache_size) {
        cache_size *= 2;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) exit(1);
    }
}

/* fill the cache until it contains at least n terms */
static void ensure_cache(idx_t n) {
    if (n <= cache_count) return;  /* already computed */

    while (cache_count < n) {
        /* test next_candidate */
        if (valid_base4(next_candidate)) {
            grow_cache();
            cache[cache_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;    /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n-1];
}
