/*
 * A043881  Numbers k such that 0 and 2 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043881:
   numbers k such that 0 and 2 occur juxtaposed in the base‑4 representation of k
   but not of k+1.  The function a(n) returns the n‑th such number
   (a(1)=8, a(2)=18, …).  The implementation scans integers in order,
   caching found terms until the requested index is reached.  It works
   for any seq_t/idx_t combination provided by the build. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state used to generate and cache the terms */
static seq_t *cache = NULL;      /* dynamic array of found terms */
static size_t cache_len = 0;     /* number of terms cached so far */
static size_t cache_cap = 0;     /* current capacity of the cache array */
static seq_t next_k = 0;         /* next integer to examine */

/* returns true if the base‑4 representation of x contains an adjacent
   pair of digits 0 and 2 (in either order) */
static bool has_adjacent(seq_t x)
{
    if (x == 0)
        return false;
    uint8_t prev = 4;                 /* sentinel value */
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 4);
        if ((prev == 0 && d == 2) || (prev == 2 && d == 0))
            return true;
        prev = d;
        x /= 4;
    }
    return false;
}

/* Ensure that at least `need` terms are cached. */
static void ensure_cache(size_t need)
{
    if (need <= cache_len)
        return;

    /* lazily allocate the cache with a default capacity */
    if (cache_cap == 0) {
        cache_cap = 20000; /* initial guess */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
    }

    while (cache_len < need) {
        seq_t k = next_k;
        if (has_adjacent(k) && !has_adjacent(k + 1)) {
            if (cache_len == cache_cap) {           /* expand capacity */
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = k;
        }
        ++next_k;
    }
}

/* Public API required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1 – ignore n = 0 */
        return 0;
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
