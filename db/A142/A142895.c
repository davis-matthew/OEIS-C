/*
 * A142895  Primes congruent to 11 mod 63.
 * https://oeis.org/A142895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 177, u32 n <= 123, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142895: Primes congruent to 11 mod 63.
 * Computes a(n) by generating the nth prime p with p % 63 == 11.
 * The function maintains a cache of found primes and continues
 * searching from where it left off on successive calls.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t cand = 11;      /* next candidate to test */
static bool first_call = true;  /* initialise cand once */

/* Ensure the cache can hold at least 'needed' entries. */
static void
ensure_cache_cap(size_t needed)
{
    if (needed > cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)            /* undefined for 0, return 0 for safety */
        return (seq_t)0;

    if (first_call) {
        first_call = false;
        cand = 11;         /* starting value */
    }

    /* Generate primes until we have at least n of them */
    while (cache_len < n)
    {
        if (is_prime_big(cand))
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = cand;
        }
        cand += 63;        /* move to next number congruent to 11 (mod 63) */
    }

    return cache[(size_t)(n - 1)];
}
