/*
 * A046954  Numbers k such that 6*k + 1 is nonprime.
 * https://oeis.org/A046954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 6*k+1 is non‑prime (composite or 1)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is increasing and a(1)=0, a(2)=4, a(3)=8, ... */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Cache to store computed terms. */
static seq_t *cache = NULL;       /* pointer to array of seq_t */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of computed terms */

/* The last k that was found (cache[cache_len-1] when cache_len>0). */
static idx_t last_k = 0;

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t new_capacity = cache_cap == 0 ? 16 : cache_cap;
    while (new_capacity < new_cap)
        new_capacity *= 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    /* In the unlikely event that realloc fails we simply keep using the old
       buffer; the function is assumed to be called only with modest sizes
       (n <= 10000) so the chance of failure is negligible. */
    if (new_ptr)
        cache = new_ptr, cache_cap = new_capacity;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        idx_t k;
        if (cache_len == 0) {
            /* First term a(1)=0 */
            k = 0;
        } else {
            k = last_k + 1;
        }

        /* Find the next k such that 6*k+1 is not prime. */
        for (;;) {
            seq_t val = 6 * (seq_t)k + (seq_t)1;
            if (!isPrimeGeneric(val))
                break;          /* val is non‑prime; k is a valid term */
            ++k;
        }

        /* Store the new term. */
        grow_cache(cache_len + 1);
        cache[cache_len] = (seq_t)k;
        ++cache_len;
        last_k = k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined but safe fallback */

    ensure_terms(n);
    return cache[n - 1];
}
