/*
 * A155878  a(0)=4; for n > 0, a(n) is the smallest composite number c >
 *          a(n-1) such that c + n is also composite.
 * https://oeis.org/A155878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4; for n > 0, a(n) = smallest composite c > a(n‑1)
 * such that c + n is also composite. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* cache[i] = a(i) for 0 ≤ i < cache_len */
static size_t cache_len = 0;  /* number of stored terms (next free index) */

seq_t a(idx_t n)
{
    /* initialise cache with a(0) if empty */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* if the requested term is already cached, return it */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* extend cache up to the requested index */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t c = cache[i - 1] + (seq_t)1;   /* start searching just above previous term */
        for (;;) {
            /* test compositeness: a number is composite iff it is not prime
               and > 1 (c starts from 4). */
            if (!is_prime_big(c) && !is_prime_big(c + (seq_t)i)) {
                cache[i] = c;
                break;
            }
            ++c;
        }
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
