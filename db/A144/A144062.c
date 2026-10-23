/*
 * A144062  a(1)=1; for n>1, a(n) = least integer > a(n-1) such that
 *          a(n)^2-a(n-1)^2 = semiprime.
 * https://oeis.org/A144062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; for n>1, a(n)=min{x>a(n-1) | x^2 - a(n-1)^2 is a
 *            semiprime}
 * Range:     u8 n <= 8, u16 n <= 280, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_semiprime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_semiprime_seq.h"

static seq_t* cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* not used */

    /* Initialise cache with a(1)=1 on first call. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;
    }

    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + (seq_t)1;

        /* Find the next a(n). */
        while (1) {
            seq_t diff2 = cand * cand - prev * prev;
            if (is_semiprime_seq(diff2))
                break;
            cand += (seq_t)1;
        }

        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = cand;
    }

    return cache[(size_t)(n - 1)];
}
