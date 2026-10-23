/*
 * A033899  Sort then Add, a(1)=17.
 * https://oeis.org/A033899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=17, a(n)=a(n-1)+sort_ascending_digits(a(n-1))
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   sort_digits_asc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sort_digits_asc.h"

/* Allocate a static cache large enough for the expected usage.
   The sequence is required to be computed up to about 10 000, so
   we preallocate that many terms. If a larger n is requested,
   the function will fall back to dynamic allocation. */
#define MAX_CACHE_SIZE 10000

static seq_t cache[MAX_CACHE_SIZE + 1] = {0};
static idx_t cached_terms = 0; /* number of terms stored in 'cache' */

static seq_t *dynamic_cache = NULL;
static idx_t dynamic_cached_terms = 0;
static size_t dynamic_capacity = 0;

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Use static cache when possible */
    if (cached_terms > 0 && n <= MAX_CACHE_SIZE) {
        if (n <= cached_terms) {
            return cache[n - 1];
        }
        for (idx_t i = cached_terms; i < n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev + sort_digits_asc(prev);
        }
        cached_terms = n;
        return cache[n - 1];
    }

    /* If we need more than the static cache can hold, use dynamic cache */
    if (dynamic_cache == NULL) {
        dynamic_capacity = (size_t)n > 1 ? (size_t)n : 1;
        dynamic_cache = (seq_t *)malloc(dynamic_capacity * sizeof(seq_t));
        dynamic_cache[0] = (seq_t)17;
        dynamic_cached_terms = 1;
    }

    if (n > dynamic_cached_terms) {
        if ((size_t)n > dynamic_capacity) {
            size_t new_cap = dynamic_capacity;
            while ((size_t)n > new_cap) {
                new_cap <<= 1;
            }
            dynamic_cache = (seq_t *)realloc(dynamic_cache,
                                            new_cap * sizeof(seq_t));
            dynamic_capacity = new_cap;
        }
        for (idx_t i = dynamic_cached_terms; i < n; ++i) {
            seq_t prev = dynamic_cache[i - 1];
            dynamic_cache[i] = prev + sort_digits_asc(prev);
        }
        dynamic_cached_terms = n;
    }

    return dynamic_cache[n - 1];
}
