/*
 * A117241  Numbers divisible by the sum of k times the k-th digit.
 * https://oeis.org/A117241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers divisible by the sum of k × k‑th digit (weights start at
   1 for the leftmost digit). This implementation searches successive
   integers and keeps those that satisfy the divisibility condition.
   The sequence starts with 1, 2, 3, … as required. */

#include <stdlib.h>
#include <stdbool.h>

/* Cached values of the sequence. */
static seq_t *cache = NULL;      /* array of a(i) values */
static size_t cache_size = 0;    /* number of entries stored */
static size_t cache_cap   = 0;    /* allocated capacity */
static seq_t next_candidate = 1; /* next integer to test */

/* Grow the cache if it doesn't have the requested number of elements. */
static void grow_cache(size_t needed)
{
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < needed) new_cap <<= 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Ensure we have at least n values computed. */
    while (cache_size < n) {
        seq_t x = next_candidate;

        /* Count digits of x. */
        seq_t temp = x;
        size_t len = 0;
        do {
            temp /= 10;
            ++len;
        } while (temp != 0);

        /* Compute weighted digit sum: leftmost digit weight 1. */
        seq_t sum = 0;
        temp = x;
        for (size_t i = len; i >= 1; --i) {
            seq_t digit = temp % 10;
            sum += digit * (seq_t)i;
            temp /= 10;
        }

        /* Accept if divisible by the weighted sum. */
        if (sum != 0 && (next_candidate % sum) == 0) {
            if (cache_size >= cache_cap) grow_cache(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
