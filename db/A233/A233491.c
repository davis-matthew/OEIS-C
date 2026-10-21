/*
 * A233491  Number of (n+2)X(2+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally or antidiagonally downwards.
 * https://oeis.org/A233491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 69*a(n-1) + 18*a(n-2) – 1125*a(n-3) + 108*a(n-4) +
 *            4131*a(n-5) – 3402*a(n-6) + 729*a(n-7) Initial values:
 *            a(1)=388908, a(2)=26851581, a(3)=1853443458, a(4)=127934319483,
 *            a(5)=8830686982464, a(6)=609539583118581, a(7)=42073567268258250
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding a(1),a(2),… */
static size_t cache_len = 0;     /* number of values actually stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* ensure the cache can hold at least n terms (1‑based indexing) */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_len)
        return;

    /* grow the storage if necessary */
    if ((size_t)n > cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 16;
        while (newcap < (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }

    /* initial seeds */
    static const seq_t init[7] = {
        (seq_t)388908ULL,
        (seq_t)26851581ULL,
        (seq_t)1853443458ULL,
        (seq_t)127934319483ULL,
        (seq_t)8830686982464ULL,
        (seq_t)609539583118581ULL,
        (seq_t)42073567268258250ULL
    };

    /* fill in new entries */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t val;
        if (i < 7) {
            val = init[i];
        } else {
            /* recurrence: a(i+1) = ...
               note: i indexes 0‑based, so a(i+1) maps to cache[i]      */
            val = 69 * cache[i-1]
                + 18 * cache[i-2]
                - 1125 * cache[i-3]
                + 108 * cache[i-4]
                + 4131 * cache[i-5]
                - 3402 * cache[i-6]
                + 729 * cache[i-7];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* a(0) is outside the official offset */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
