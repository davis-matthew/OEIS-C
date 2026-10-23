/*
 * A033652  Trajectory of 13 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 13; a(n+1) = a(n) + rev_decimal(a(n)) */

#include <stddef.h>
#include <stdlib.h>

/* Reverse the decimal digits of a value.
 * Works for any seq_t that provides +, *, /, % and > comparision. */
static inline seq_t rev_decimal(seq_t v)
{
    seq_t rev = (seq_t)0;
    while (v > (seq_t)0) {
        seq_t d = v % (seq_t)10;
        rev = rev * (seq_t)10 + d;
        v = v / (seq_t)10;
    }
    return rev;
}

/* Cached sequence values; static so subsequent calls use previously
 * computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Compute a(n) for any n >= 0 using memoisation.
 * The function is safe for arbitrary ordering of calls. */
seq_t a(idx_t n)
{
    /* Convert idx_t to a size_t that can index the cache. */
    size_t idx = (size_t)n;

    /* Grow cache if necessary. */
    if (cache_size <= idx) {
        size_t new_cap = idx + 1;
        /* C++ requires an explicit cast from void* to seq_t*. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: return 0 as a sentinel. */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise new entries. */
        for (size_t i = cache_size; i < new_cap; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)13;
            } else {
                seq_t prev = cache[i - 1];
                seq_t rev = rev_decimal(prev);
                cache[i] = prev + rev;
            }
        }
        cache_size = new_cap;
    }

    return cache[idx];
}
