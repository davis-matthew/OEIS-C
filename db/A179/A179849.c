/*
 * A179849  Sum of prime p and next prime after p is divisible by 7.
 * https://oeis.org/A179849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p where p is the first of a consecutive prime pair (p, q)
 *            * with (p + q) divisible by 7. The sequence is defined for
 *            offset 1 * (a(1) = 19). This implementation searches primes on
 *            demand and * memoises already-found terms
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided is_prime utility. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/is_prime.h"

/* External linkage, as required. */
seq_t a(idx_t n)
{
    /* 1‑based cache of computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_sz   = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* capacity of the cache array */

    /* State that persists between calls to keep the search efficient. */
    static uint64_t prev_prime = 2; /* last prime seen during the search */
    static uint64_t candidate  = 3; /* current odd candidate to test */

    /* a(0) is undefined for this sequence; return 0 for completeness. */
    if (n == 0) return (seq_t)0;

    /* Resize the cache if we need to store more terms. */
    if (n > cache_sz && cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_sz < n) {
        if (is_prime((idx_t)candidate)) {
            if ((prev_prime + candidate) % 7ULL == 0ULL) {
                /* Found a qualifying pair; store the first prime (prev_prime). */
                cache[cache_sz++] = (seq_t)prev_prime;

                /* Update state for the next search. */
                prev_prime = candidate;
                candidate += 2U;
                if (cache_sz >= n) break;
                continue;  /* skip the unconditional increment below */
            }
            /* Not a qualifying pair – move on. */
            prev_prime = candidate;
        }
        candidate += 2U;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
