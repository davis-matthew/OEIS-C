/*
 * A037424  Positive numbers having the same set of digits in base 4 and base
 *          6.
 * https://oeis.org/A037424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose set of digits in base 4
 *            equals its set of digits in base 6. The function builds the
 *            sequence lazily and reuses previously computed terms
 * Range:     u8 n <= 9, u16 n <= 1901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   same_digit_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/same_digit_set.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Lazily extend cache until it contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    seq_t cand = cache_len ? cache[cache_len-1] + 1 : 1;

    while (cache_len < (size_t)n) {
        if (same_digit_set(cand))
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
        if (same_digit_set(cand)) {
            cache[cache_len++] = cand;
            /* if already added, increment next candidate after storing */
            cand++;
            continue;
        }
        cand++;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;   /* out of range */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
