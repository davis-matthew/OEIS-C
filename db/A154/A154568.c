/*
 * A154568  Composites of the form (sum of digits of k) + (sum of digits of
 *          prime(k)).
 * https://oeis.org/A154568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th composite integer of the form sum_of_digits(k) +
 *            sum_of_digits(prime(k)), k ≥ 1
 * Range:     u8 n <= 38, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   digit_sum, get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;
static size_t  cache_len = 0, cache_cap = 0;
static idx_t   next_k = 1;
static bool    initialized = false;

/* Ensure the cache array can hold at least `cap` elements. */
static void ensure_cache_cap(size_t cap)
{
    if (cache_cap >= cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < cap) new_cap = cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Sum of decimal digits of an idx_t value. */
static seq_t digit_sum_idx(idx_t x)
{
    seq_t s = 0;
    while (x) {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* Return true iff v is a composite integer (≥4 and not prime). */
static bool is_composite(seq_t v)
{
    if (v < 4) return false;          /* 0,1,2,3 are not composite */
    return !isPrimeGeneric(v);         /* isPrimeGeneric returns true if prime */
}

/* ---
   External function required by the OEIS build system. 
   Computes the n-th term of sequence A154568. 
   The function is memoised, so subsequent calls are fast. 
   --- */
seq_t a(idx_t n)
{
    /* Initialise static data structures if this is the first call. */
    if (!initialized) {
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        next_k = 1;
        initialized = true;
    }

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        /* Compute prime(k) and the required digit sums. */
        uint32_t prime_k = get_prime(next_k);
        seq_t    sum_k   = digit_sum_idx(next_k);
        seq_t    sum_p   = digit_sum((seq_t)prime_k);
        seq_t    total   = sum_k + sum_p;

        /* Store the composite sums. */
        if (is_composite(total)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = total;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
