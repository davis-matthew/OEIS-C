/*
 * A023731  Numbers with exactly two 2's in base 5 expansion.
 * https://oeis.org/A023731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose base‑5 representation
 *            contains exactly two digits equal to 2
 * Range:     u8 n <= 25, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 64 terms through n = 2835 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper: test whether a number has exactly two ‘2’ digits in base‑5 */
static bool has_two_twos(seq_t v)
{
    unsigned cnt = 0;
    while (v != 0) {
        if ((v % 5) == 2) {
            ++cnt;
            if (cnt > 2) return false;
        }
        v /= 5;
    }
    return cnt == 2;
}

/* static cache to hold already found terms */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_size = 0;     /* number of terms stored */
static seq_t next_candidate = 1;  /* integer to start searching from */

/* enlarge cache when needed */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
        next_candidate = 1;
    }

    /* fill cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        while (true) {
            if (has_two_twos(next_candidate)) {
                ensure_cache_capacity(cache_size + 1);
                cache[cache_size++] = next_candidate;
                ++next_candidate;   /* next search begins with the following integer */
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
