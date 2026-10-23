/*
 * A127590  Numbers n such that 16n+5 is prime.
 * https://oeis.org/A127590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that 16*n+5 is prime; returns the nth such n (1‑based index). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache: cache[i] = nth n with 16*n+5 prime, 0‑based i */
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;
    static size_t cache_len    = 0;
    /* last candidate n that was tested (inclusive) */
    static idx_t last_checked = (idx_t)-1;   /* wrap‑around to 0 on first increment */

    if (n == 0)   /* undefined for 0, return 0 for safety */
        return (seq_t)0;

    /* grow cache until we have at least n terms */
    while (cache_len < n)
    {
        idx_t cand = last_checked + 1;   /* next candidate n, wrap‑around handled above */
        last_checked = cand;

        idx_t p = (idx_t) ((uint64_t)16 * cand + 5);
        if (is_prime(p))
        {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)cand;
            cache_len++;
        }
    }
    return cache[n - 1];
}
