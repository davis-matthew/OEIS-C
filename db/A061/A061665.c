/*
 * A061665  Numbers n whose digits in their prime factorization are the same
 *          as those of the prime factorization of n+1.
 * https://oeis.org/A061665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digit_counts, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A061665: 
   Numbers n whose digits in their prime factorizations are the same 
   as those of the prime factorization of n+1. 
   The implementation searches for terms incrementally and caches 
   them so that any n≥0 is returned in O(1) after the initial search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_counts.h"

/* Local cache of found terms as uint64_t (they fit comfortably in 
   64 bits for the range of indices expected). */
static uint64_t *cache_u64 = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_u64_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *ptr = (uint64_t *)realloc(cache_u64, new_cap * sizeof(uint64_t));
    if (!ptr)
        abort();           /* allocation failure */
    cache_u64 = ptr;
    cache_cap = new_cap;
}

/* Count the decimal digits present in the prime factorization of n.
   Resulting counts are stored in out[10]. */
static void factor_digit_counts(uint64_t n, unsigned int out[10])
{
    unsigned int local[10];
    memset(local, 0, sizeof(local));

    size_t cnt;
    uint64_t primes[64], exps[64];
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        unsigned char dcnt[10];
        digit_counts((seq_t)primes[i], dcnt);
        for (int d = 0; d < 10; ++d) {
            if (dcnt[d])
                local[d] += dcnt[d] * exps[i];
        }
    }
    memcpy(out, local, sizeof(local));
}

/* Initialise the cache with the first known term. */
static void initialise_cache(void)
{
    if (cache_len > 0)
        return;
    ensure_cache_u64_size(1);
    cache_u64[0] = 8763;   /* a(0) */
    cache_len = 1;
}

/* Return the nth term (0‑based). */
seq_t a(idx_t n)
{
    initialise_cache();

    if ((size_t)n < cache_len)
        return (seq_t)cache_u64[(size_t)n];

    /* Search for additional terms until we have enough. */
    uint64_t cand = cache_u64[cache_len - 1] + 1;
    unsigned int curr[10], next[10];

    factor_digit_counts(cand, curr);
    factor_digit_counts(cand + 1, next);

    while (cache_len <= (size_t)n) {
        if (memcmp(curr, next, 10 * sizeof(unsigned int)) == 0) {
            ensure_cache_u64_size(cache_len + 1);
            cache_u64[cache_len++] = cand;
        }
        ++cand;
        memcpy(curr, next, 10 * sizeof(unsigned int));
        factor_digit_counts(cand + 1, next);
    }

    return (seq_t)cache_u64[(size_t)n];
}
