/*
 * A230123  Smallest number of the form 11*m+1 with exactly n prime factors,
 *          counted with multiplicity.
 * https://oeis.org/A230123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-10) * 2^10 for n >= 13
   Base cases are the smallest numbers ≡ 1 (mod 11) having exactly n prime
   factors (counted with multiplicity). */
/* The function is memoized and works for any order of calls. */

#include <stddef.h>      /* size_t, idx_t is provided by the build */
#include <stdlib.h>      /* malloc, realloc, calloc */
#include <string.h>      /* memset */
#include "../utils/count_prime_factors.h"

static seq_t* cache = NULL;      /* memoization table */
static size_t cache_capacity = 0;

/* Ensure cache can hold index n (zero‑based).  New entries are initialized to 0. */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_capacity = 1;
        cache = (seq_t*)calloc(cache_capacity, sizeof(seq_t));
        return;
    }
    if ((size_t)n + 1 <= cache_capacity) return;

    size_t new_capacity = cache_capacity;
    while ((size_t)n + 1 > new_capacity) new_capacity <<= 1;

    seq_t* new_cache = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
    if (new_cache != cache) {
        /* realloc may have moved the block; the new part is not initialized */
    }
    memset(new_cache + cache_capacity, 0,
           (new_capacity - cache_capacity) * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Brute‑force search for n < 13:
   smallest x ≡ 1 (mod 11) with Ω(x) = n. */
static seq_t brute_min(idx_t n)
{
    seq_t m = 0;
    while (1) {
        seq_t candidate = m * (seq_t)11 + (seq_t)1;
        if (count_prime_factors(candidate) == n) return candidate;
        ++m;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* Return cached value if available */
    if (n > 0 && cache[n] != (seq_t)0) return cache[n];

    seq_t result;
    if (n == 0) {
        result = (seq_t)1;
    } else if (n < 13) {
        result = brute_min(n);
    } else {
        seq_t prev = a(n - 10);
        result = prev * ((seq_t)1024);   /* 2^10 = 1024 */
    }

    cache[n] = result;
    return result;
}
