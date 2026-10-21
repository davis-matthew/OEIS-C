/*
 * A065200  Numbers of the form m * p^k, with p prime, k >= 0, m squarefree
 *          and p > any prime factor of m.
 * https://oeis.org/A065200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that is either squarefree or
 *            equals m * p^k with p prime, k ≥ 0, m squarefree and p strictly
 *            larger than every prime factor of m
 * Range:     u8 n <= 183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* ------------------------------------------------------------------ */
/* Helper that decides whether a number satisfies the A065200 condition */
/* ------------------------------------------------------------------ */
static bool a065200_ok(uint64_t x)
{
    /* 1 is squarefree by definition. */
    if (x == 1) return true;

    /* Extract prime factorisation.  The factor_u64 routine
       populates the arrays in ascending order (guaranteed by the
       implementation).  We keep a fixed-size buffer large enough
       for any 64‑bit integer (≤ 64 distinct primes). */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(x, pr, ex, &cnt);

    /* Count primes that occur more than once.  If more than one
       such prime exists, the number does not belong to the
       sequence. */
    size_t rep_cnt = 0;
    uint64_t rep_p = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > 1) {
            ++rep_cnt;
            rep_p = pr[i];
            if (rep_cnt > 1) return false;
        }
    }

    /* Squarefree numbers (rep_cnt == 0) are allowed. */
    if (rep_cnt == 0) return true;

    /* Now exactly one prime has exponent > 1.  That prime must be
       larger than every other prime factor. */
    uint64_t maxp = pr[0];
    for (size_t i = 1; i < cnt; ++i)
        if (pr[i] > maxp)
            maxp = pr[i];

    return rep_p == maxp;
}

/* ------------------------------------------------------------------ */
/* Cache for the sequence values.  The data are stored in a dynamic
   array that expands as needed.  The array is filled lazily: when a
   call to a(idx_t n) queries a value beyond the current length,
   additional integers are tested until the requested index is
   reached. */
/* ------------------------------------------------------------------ */
static seq_t  *cache   = NULL;   /* array of cached values                */
static size_t   cache_len  = 0;   /* number of values stored               */
static size_t   cache_cap  = 0;   /* current allocated capacity            */
static uint64_t next_value = 1;   /* next integer to test for inclusion   */

/* Ensure that the cache has at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_len >= need) return;

    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();           /* out of memory */
    }

    for (; cache_len < need; ++next_value) {
        if (a065200_ok(next_value)) {
            if (cache_len == cache_cap) {
                /* Grow the array (doubling strategy). */
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_value;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public API required by the test harness.  The function returns the
   n-th term (with the offset 1).  It may be called nondeterministically
   – the cache guarantees correct results in any order. */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index 0 is outside the offset; return 0 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
