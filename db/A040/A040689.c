/*
 * A040689  Continued fraction for sqrt(716).
 * https://oeis.org/A040689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(716) */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* continued‑fraction state variables */
    static seq_t a0 = 0;             /* floor(sqrt(716)) */
    static unsigned long long M = 0; /* previous m */
    static unsigned long long D = 1; /* previous d */
    static seq_t a_prev = 0;         /* previous a */

    /* one‑time initialisation */
    if (!initialized) {
        a0 = floor_sqrt((seq_t)716);
        cache_cap = 4;                     /* initial capacity */
        cache = mem_new(seq_t, cache_cap);
        cache[0] = a0;
        cache_len = 1;
        M = 0;
        D = 1;
        a_prev = a0;
        initialized = true;
    }

    /* return cached value if already available */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* generate terms up to requested index */
    while (cache_len <= (size_t)n) {
        unsigned long long nextM = D * (unsigned long long)a_prev - M;
        unsigned long long nextD = (716ULL - nextM * nextM) / D;
        unsigned long long nextA_ll = ((unsigned long long)a0 + nextM) / nextD;
        seq_t nextA = (seq_t)nextA_ll;

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        /* store new term and update state */
        cache[cache_len] = nextA;
        cache_len++;

        M = nextM;
        D = nextD;
        a_prev = nextA;
    }

    return cache[(size_t)n];
}
