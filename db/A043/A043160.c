/*
 * A043160  Numbers k such that 1 and 3 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1849, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043160
 *  Numbers k such that the digits 1 and 3 occur next to each other
 *  (in either order) in the base‑8 representation of k but not in
 *  the base‑8 representation of k–1.  The function generates the
 *  sequence on demand, caching results so that calls with arbitrary
 *  values of n are answered efficiently.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: return true if v (an idx_t value) has the digits
 * 1 and 3 adjacent in its base‑8 representation (either 13 or 31). */
static bool has_adjacent13(const idx_t v)
{
    if (v == 0) return false;

    int prev = -1;
    idx_t x = v;

    /* Iterate over base‑8 digits from least significant to most. */
    while (x != 0) {
        int digit = (int)(x & 7);          /* x % 8  (bitwise) */
        if ((prev == 3 && digit == 1) ||
            (prev == 1 && digit == 3))
            return true;
        prev = digit;
        x >>= 3;                           /* x /= 8  (bitwise) */
    }
    return false;
}

/*  a(n) returns the nth term of OEIS A043160 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range – behaviour undefined */

    /* Static cache: values already found, next k to test, and cache size. */
    static idx_t *cache      = NULL;      /* array of found k's (idx_t works for all builds) */
    static size_t cache_used = 0;         /* number of cached values */
    static size_t cache_cap  = 0;         /* allocated capacity of cache */
    static idx_t next_k      = 1;         /* next k to examine */

    /* Grow cache to at least N items. */
    while (cache_used < n) {
        idx_t k = next_k;

        if (has_adjacent13(k) && !has_adjacent13(k - 1)) {
            /* Ensure enough space. */
            if (cache_used == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                idx_t *new_cache = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                if (!new_cache) {               /* allocation failed */
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_used++] = k;
        }
        next_k++;
    }

    return (seq_t)cache[n - 1];
}
