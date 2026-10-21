/*
 * A095985  Numbers k such that k * (largest digit of k) is a triangular
 *          number.
 * https://oeis.org/A095985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _floor_isqrt_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095985: Numbers k such that k * (largest digit of k) is a triangular number. */
/* The function a(n) is defined for n ≥ 1 (a(1) = 0). */
/* Implementation: scan successive integers k, test the triangularity of
   k * (largest digit of k) using the characterisation: 8*m+1 is a perfect
   square, where m = k * largest_digit(k).  Matching terms are cached so
   that calls in any order produce the same result. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* include the utilities needed for the cache handling and sqrt check */
#include "../utils/seq_realloc.h"      /* seq_realloc() */
#include "../utils/_floor_isqrt_u64.h" /* _floor_isqrt_u64() */

/* Cache and state ----------------------------------------------------------*/
static seq_t *cache = NULL;      /* stored sequence values */
static size_t cache_cap = 0;     /* allocated size of cache     */
static size_t cache_size = 0;    /* number of terms stored      */
static uint64_t next_k = 0;      /* smallest k not yet tested   */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the largest decimal digit of a non‑negative integer x. */
static uint8_t largest_digit(uint64_t x)
{
    if (x == 0)
        return 0;
    uint8_t maxd = 0;
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d > maxd)
            maxd = d;
        x /= 10;
    }
    return maxd;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(1) is the first term given. */
    if (n == 0)
        return (seq_t)0;            /* undefined, return 0 */

    size_t target = (size_t)n;      /* number of terms that must be present */

    /* Generate terms until the cache holds at least 'target' elements. */
    while (cache_size < target) {
        /* Search for the next qualifying integer. */
        uint64_t k = next_k;
        while (true) {
            uint8_t mxd = largest_digit(k);
            unsigned __int128 m = (unsigned __int128)k * (unsigned __int128)mxd;
            unsigned __int128 t = 8 * m + 1;           /* 8*m + 1 */

            uint64_t t64 = (uint64_t)t;                /* fits for our range */
            uint64_t s = _floor_isqrt_u64(t64);       /* integer sqrt */

            /* m is triangular iff 8*m+1 is a perfect square */
            if ((unsigned __int128)s * (unsigned __int128)s == t) {
                /* Record the term. */
                ensure_capacity(cache_size + 1);
                cache[cache_size++] = (seq_t)k;
                break;
            }
            ++k;                                       /* next candidate */
        }

        next_k = k + 1;                                 /* start after found */
    }

    return cache[target - 1];                            /* 1‑based indexing */
}
