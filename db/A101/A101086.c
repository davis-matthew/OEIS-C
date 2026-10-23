/*
 * A101086  Numbers k for which 997*k + 1009 is prime.
 * https://oeis.org/A101086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 997*k + 1009 is prime */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_len = 0;
static idx_t next_k = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset handling – not used for n >= 1 */

    if (!cache) {
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)0;           /* a(1) = 0 */
        cache_len = 1;
        next_k = 1;                    /* start searching from k = 1 */
    }

    while (cache_len < n) {
        uint64_t cand = 997ULL * (uint64_t)next_k + 1009ULL;
        if (is_prime_u64(cand)) {
            if (cache_len == cache_capacity) {
                cache_capacity <<= 1;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
                if (!cache) return 0;  /* allocation failure */
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
