/*
 * A039433  Numbers whose base-10 representation has the same nonzero number
 *          of 6's and 8's.
 * https://oeis.org/A039433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose decimal representation contains an equal nonzero
   number of 6's and 8's.  The function searches integers, accumulating
   results until the required index is reached. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static int equal_6_8(uint64_t x) {
    int c6 = 0, c8 = 0;
    while (x > 0) {
        int d = x % 10;
        if (d == 6) ++c6;
        else if (d == 8) ++c8;
        x /= 10;
    }
    return (c6 > 0 && c8 > 0 && c6 == c8);
}

static uint64_t *cache = NULL;   /* cache of found values */
static size_t   cache_cap = 0;   /* allocated size of cache */
static size_t   cache_len = 0;   /* number of values currently stored */
static uint64_t next_start = 1;  /* next integer to check */

static void grow_cache(void) {
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (uint64_t*)malloc(cache_cap * sizeof(uint64_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (uint64_t*)realloc(cache, cache_cap * sizeof(uint64_t));
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */

    /* Generate until we have enough terms */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_start++;
        if (equal_6_8(cand)) {
            grow_cache();
            cache[cache_len++] = cand;
        }
    }
    return (seq_t)cache[n - 1];
}
