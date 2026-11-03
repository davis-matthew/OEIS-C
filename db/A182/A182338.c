/*
 * A182338  List of positive integers whose prime tower factorization, as
 *          defined in comments, contains the prime 3.
 * https://oeis.org/A182338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9987 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A182338: numbers whose prime‑tower factorization contains the prime 3.  
 * The function searches forward from the start of the sequence, caching results
 * so that arbitrary order of calls is handled efficiently. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Recursive test: does the prime–tower factorization of x contain prime 3? */
static bool has3(uint64_t x)
{
    if (x <= 1) {
        return false;
    }

    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            if (p == 3) {
                return true;                /* base prime 3 present */
            }
            if (e > 1 && has3(e)) {
                return true;                /* exponent’s tower contains 3 */
            }
        }
    }
    /* Remaining prime factor, if any. */
    if (n > 1) {
        return (n == 3);                    /* remaining prime 3 */
    }
    return false;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    /* Static cache for generated sequence values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static uint64_t next_candidate = 3;      /* start search at 3 */

    /* Build the cache up to the requested index */
    while (cache_len < (size_t)n) {
        /* Find the next integer whose prime tower factorization contains 3 */
        while (!has3(next_candidate)) {
            ++next_candidate;
        }

        /* Grow the cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                abort();                  /* allocation failure */
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the candidate and advance */
        cache[cache_len] = (seq_t)next_candidate;
        ++cache_len;
        ++next_candidate;
    }

    return cache[n - 1];
}
