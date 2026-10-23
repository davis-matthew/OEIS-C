/*
 * A121058  Positive integers x such that x+d+1 is composite for all divisors
 *          d of x.
 * https://oeis.org/A121058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A121058: Positive integers x such that x + d + 1 is composite for all divisors d of x. */
/* The algorithm performs a brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utilities provided by the build system */
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;         /* cache of found terms */
static size_t cache_len = 0;        /* number of terms cached */
static size_t cache_cap = 0;        /* current allocation capacity */
static seq_t next_check = 2;        /* smallest candidate not yet tested */

/* Test whether a single integer satisfies the definition. */
static bool is_candidate(seq_t x)
{
    /* limit = floor(sqrt(x)) */
    seq_t sqrt_x = floor_sqrt(x);
    idx_t limit = (idx_t)sqrt_x;   /* safe: limit is small for our use case */

    for (idx_t d = 1; d <= limit; ++d) {
        if ((x % (seq_t)d) == 0) {
            seq_t y1 = x + (seq_t)d + 1;
            if (isPrimeGeneric(y1))
                return false;

            seq_t d2 = x / (seq_t)d;
            if (d2 != (seq_t)d) {
                seq_t y2 = x + d2 + 1;
                if (isPrimeGeneric(y2))
                    return false;
            }
        }
    }
    return true;
}

/* Reallocate cache if needed. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a production environment we would handle the allocation error.
           For the purposes of this sequence generator we simply abort. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function that returns the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, defensive guard */

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Find terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cand = next_check++;
        if (is_candidate(cand)) {
            if (cache_len == cache_cap)
                grow_cache(cache_cap * 2);
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
