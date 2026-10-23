/*
 * A138255  Smallest positive integer m such that n divides [2^m/m]
 *          (=A000799(m)).
 * https://oeis.org/A138255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest m such that n divides floor(2^m/m)      */
/* a(n) = min { m ≥ 1 | n | ⌊ 2^m / m ⌋ }           */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* cache for memoisation; works for both C and C++ */
struct cache_entry {
    idx_t n;
    seq_t m;
};

static struct cache_entry *cache = NULL;   /* list of cached results */
static size_t cache_size = 0;              /* number of entries used   */
static size_t cache_cap  = 0;              /* allocated capacity       */

/* enlarge cache storage as required */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        if (new_cap < need) new_cap = need;
        struct cache_entry *tmp = (struct cache_entry *)realloc(cache, new_cap * sizeof(*tmp));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* linear search cache for a given n; return 1 if found */
static int cache_find(idx_t n, seq_t *out)
{
    for (size_t i = 0; i < cache_size; ++i) {
        if (cache[i].n == n) {
            *out = cache[i].m;
            return 1;
        }
    }
    return 0;
}

/* insert a new (n, m) pair into the cache */
static void cache_insert(idx_t n, seq_t m)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size].n = n;
    cache[cache_size].m = m;
    ++cache_size;
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, safe fall‑back */

    seq_t cached;
    if (cache_find(n, &cached)) return cached;

    for (idx_t m = 1; ; ++m) {
        /* modulus = m * n                              */
        seq_t mod   = (seq_t)m * (seq_t)n;
        /* T = 2^m (mod mod)                             */
        seq_t T     = mod_pow(2, m, mod);    /* relies on utils/mod_pow.h */

        /* r = T (mod m)                                 */
        seq_t r     = T % (seq_t)m;

        /* diff = T - r                                 */
        seq_t diff  = T - r;

        /* qmod = (diff / m) (mod n)    -> floor(2^m/m) (mod n) */
        seq_t qmod  = (diff / (seq_t)m) % (seq_t)n;

        if (qmod == 0) {
            seq_t result = (seq_t)m;
            cache_insert(n, result);
            return result;
        }
    }
}
