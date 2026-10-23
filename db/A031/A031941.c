/*
 * A031941  Numbers without consecutive equal base 3 digits.
 * https://oeis.org/A031941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 9346, u64 n <= 9346,
 *            u128 n <= 9346, big any n
 * Helpers:   no_consecutive_equal_digits_in_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A031941: numbers without consecutive equal base‑3 digits.
    The function a(n) returns the n‑th term (n ≥ 1).
    Implementation uses a simple cache and linear search.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits_in_base3.h"

/* --------------------------------------------------------------------- */
/* global cache state                                                   */
static seq_t *cache = NULL;          /* stored terms                     */
static size_t cache_count = 0;       /* how many terms are stored       */
static size_t cache_capacity = 0;    /* allocated array size             */
static idx_t last_candidate = 0;     /* last integer examined            */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* resize the cache array so that it can hold at least min_cap items     */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate the program */
        abort();
    }
    cache = tmp;
    cache_capacity = new_cap;
}

/* --------------------------------------------------------------------- */
/* public function: a(n) returns the n‑th term of the sequence            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0 – harmless */

    if (n <= cache_count)
        return cache[n - 1];

    /* ensure the cache can hold n elements */
    grow_cache((size_t)n);

    /* scan integers larger than the ones already examined */
    idx_t cand = last_candidate + 1;
    while (cache_count < (size_t)n) {
        if (no_consecutive_equal_digits_in_base3(cand)) {
            cache[cache_count] = (seq_t)cand;
            cache_count++;
        }
        cand++;
    }
    last_candidate = cand - 1;
    return cache[n - 1];
}
