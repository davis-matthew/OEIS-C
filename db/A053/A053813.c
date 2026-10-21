/*
 * A053813  Numbers which are an integral multiple of the sum of their proper
 *          divisors: prime and perfect numbers.
 * https://oeis.org/A053813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth smallest integer that is either a prime or a perfect number. */
/* The sequence is the ascending union of all primes and all perfect numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static const seq_t perfects[] = {
    (seq_t)6ULL,
    (seq_t)28ULL,
    (seq_t)496ULL,
    (seq_t)8128ULL,
    (seq_t)33550336ULL,
    (seq_t)8589869056ULL,
    (seq_t)137438691328ULL,
    (seq_t)2305843008139952128ULL
};
static const size_t num_perfects = sizeof(perfects) / sizeof(perfects[0]);

seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n == 0 is irrelevant. */
    if (n == 0) return 0;

    /* Static cache of generated terms in ascending order. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State for incremental generation. */
    static seq_t next_int = (seq_t)2;   /* smallest integer yet to examine */
    static size_t perfect_idx = 0;      /* next perfect number to consider */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* Ensure capacity for at least n terms. */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap < 16) new_cap = 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the required index. */
    while (cache_len < needed) {
        seq_t candidate = next_int;

        /* Include perfect numbers at the correct position. */
        if (perfect_idx < num_perfects && candidate == perfects[perfect_idx]) {
            cache[cache_len++] = candidate;
            ++perfect_idx;
        }
        /* Otherwise include primes. */
        else if (is_prime((idx_t)candidate)) {
            cache[cache_len++] = candidate;
        }
        /* Move to the next integer. */
        ++next_int;
    }

    return cache[(size_t)n - 1];
}
