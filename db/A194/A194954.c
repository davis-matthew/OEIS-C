/*
 * A194954  Slowest increasing sequence of primes such that a(1)=2,
 *          a(n)-a(n-1) is multiple of A000120(n-1).
 * https://oeis.org/A194954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1898, u32 n <= 1899, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194954: slowest increasing sequence of primes.
 *  a(1)=2, and for n>1 a(n) is the smallest prime > a(n-1) such that
 *  a(n)-a(n-1) is a multiple of the binary weight (popcount) of n-1. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* popcount of an idx_t value */
static inline idx_t popcount_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    /* portable fallback */
    idx_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

/* memoized sequence storage */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1; behaviour below offset is irrelevant */
    if (n == 0) return 0;  /* safety for C */

    /* extend cache up to n */
    while (cache_len < n) {
        idx_t idx = cache_len + 1;          /* desired index to compute */
        idx_t weight = popcount_idx(idx - 1); /* binary weight of n-1 */

        /* previous term */
        seq_t prev = (cache_len == 0) ? 2 : cache[cache_len - 1];
        if (cache_len == 0) {               /* initialise first term */
            /* a(1) = 2, already defined as prev */
            /* ensure capacity for first term */
            if (cache_cap == 0) {
                cache_cap = 4;
                cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
            }
            cache[0] = 2;
            cache_len = 1;
            continue;
        }

        /* find smallest prime > prev with difference divisible by weight */
        seq_t candidate = prev + 1;
        for (;;) {
            if (is_prime_big(candidate)) {
                seq_t diff = candidate - prev;
                if ((diff % (seq_t)weight) == 0)
                    break;
            }
            candidate = candidate + 1;
        }

        /* ensure capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}
