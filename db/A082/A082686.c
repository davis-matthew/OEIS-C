/*
 * A082686  Odd nonprime integers n which have an odd number of proper
 *          divisors.
 * https://oeis.org/A082686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth odd composite integer that is NOT a perfect
 *            square
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082686: odd nonprime integers with an odd number of proper divisors. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

static seq_t   *cache      = NULL;
static size_t   cache_len  = 0;
static size_t   cache_cap  = 0;
static idx_t    next_odd   = 3;   /* start testing from 3 */

static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= (size_t)n) return;

    /* Allocate or grow the cache array. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < (size_t)n) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Skip even numbers, keep next_odd odd. */
        if ((next_odd & 1ULL) == 0) next_odd++;

        /* Check composite (not prime). */
        if (!is_prime((idx_t)next_odd)) {
            idx_t s = int_sqrt((idx_t)next_odd);
            /* Exclude perfect squares. */
            if (s * s != (idx_t)next_odd) {
                cache[cache_len++] = (seq_t)next_odd;
            }
        }
        next_odd += 2;   /* move to the next odd candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out-of-range request */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
