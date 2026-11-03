/*
 * A087050  Square root of the largest square >1 dividing the n-th
 *          nonsquarefree number.
 * https://oeis.org/A087050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = square root of the largest square factor >1 of the * nth
 *            nonsquarefree number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9129 (2026-11-03)
 * Helpers:   build_spf, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utilities provided by the judge */
#include <stdint.h>
#include <string.h>
#include "../utils/build_spf.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static idx_t       _spf_limit   = 0;          /* size of _spf array (number of valid indices) */
static idx_t      * _spf        = NULL;       /* smallest prime factor for each index */

static size_t      _cache_len   = 0;          /* number of cached a(n) values */
static size_t      _cache_cap   = 0;          /* capacity of _cache array */
static seq_t      * _cache      = NULL;       /* cached a(n) values */

static idx_t       _next_candidate = 2;       /* next integer to test for nonsquarefree */

/* Make sure the SPF array covers indices up to `limit` (inclusive) */
static void _ensure_spf(idx_t limit)
{
    if (limit + 1 <= _spf_limit)
        return;                        /* already big enough */

    /* Choose a new size power‑of‑two >= limit+1 */
    idx_t new_limit = _spf_limit ? _spf_limit : 2;
    while (new_limit <= limit)
        new_limit <<= 1;

    free(_spf);                        /* free old array if any */
    build_spf(new_limit - 1, &_spf);   /* builds array for indices 0 .. new_limit-1 */
    _spf_limit = new_limit;            /* array valid up to new_limit-1 */
}

/* Ensure the cache array has capacity `need` */
static void _ensure_cache(size_t need)
{
    if (_cache_cap >= need)
        return;
    size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = mem_resize(seq_t, _cache, new_cap);
    if (!new_cache)
        exit(EXIT_FAILURE);
    _cache = new_cache;
    _cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* not part of OEIS offset */

    /* Return cached value if already computed */
    if (n <= _cache_len)
        return _cache[n - 1];

    /* Generate until we have computed up to n */
    idx_t cand = _next_candidate;
    while (_cache_len < n) {
        /* Ensure we can factor `cand` */
        _ensure_spf(cand);

        idx_t x = cand;
        bool nonsqfree = false;
        seq_t root_of_largest_square = (seq_t)1;   /* product of p^{floor(e/2)} */

        while (x > 1) {
            idx_t p = _spf[x];
            int    e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            if (e >= 2)
                nonsqfree = true;
            int q = e / 2;
            if (q > 0)
                root_of_largest_square =
                    seq_mul(root_of_largest_square,
                            ipow_i32((seq_t)p, q));
        }

        if (nonsqfree) {
            _ensure_cache(_cache_len + 1);
            _cache[_cache_len++] = root_of_largest_square;
        }

        ++cand;                                 /* next integer to try */
    }

    _next_candidate = cand;                     /* ready for future calls */
    return _cache[n - 1];
}
