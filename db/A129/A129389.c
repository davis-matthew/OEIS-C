/*
 * A129389  Numbers k such that the mean of 5 consecutive squares starting
 *          with k^2 is prime.
 * https://oeis.org/A129389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 22, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that (k + 2)^2 + 2 is prime (OEIS A129389). The
   implementation performs an incremental search and memoises the
   results so that calls with arbitrary n are answered efficiently. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Ensure that *ptr has room for at least 'needed' elements. */
static void ensure_capacity(seq_t **ptr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is undefined, but we guard it */

    /* Static cache for already discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t  last_k_checked = 0;   /* last k examined (inclusive) */

    /* Grow the cache until it contains at least 'n' terms. */
    while (cache_len < (size_t)n) {
        ++last_k_checked;                     /* next candidate k   */
        seq_t kp2 = (seq_t)(last_k_checked + 2);  /* k + 2   */
        seq_t val  = kp2 * kp2 + (seq_t)2;        /* (k+2)^2 + 2 */
        if (is_prime_big(val)) {
            ensure_capacity(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)last_k_checked;
        }
    }

    return cache[n - 1];
}
