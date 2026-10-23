/*
 * A039106  Numbers whose base-9 representation has the same number of 5's and
 *          6's.
 * https://oeis.org/A039106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A039106: numbers whose base‑9 representation contains an equal
   number of 5’s and 6's.  For n≥1 it returns the nth term (offset 1).
   The implementation scans integers incrementally, tests the property,
   caches found values, and returns cached results for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;        /* most recent good numbers */
static size_t cache_size = 0;      /* number of cached items      */
static size_t cache_cap  = 0;      /* allocated capacity         */
static idx_t   next_candidate = 0; /* next integer to test        */

static bool good(seq_t x)
{
    seq_t t = x;
    int c5 = 0, c6 = 0;
    while (t > 0) {
        int d = (int)(t % 9);
        if (d == 5) ++c5;
        else if (d == 6) ++c6;
        t /= 9;
    }
    return c5 == c6;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safeguard: offset is 1 */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_candidate;
        if (good(cand)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;           /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
