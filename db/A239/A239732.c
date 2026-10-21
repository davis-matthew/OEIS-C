/*
 * A239732  Primes p such that 2p+9 and 9p+2 are both prime.
 * https://oeis.org/A239732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 244, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds primes p such that 2p+9 and 9p+2 are also prime (OEIS A239732). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Cached sequence storage */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;      /* number of terms stored */
static size_t cache_cap   = 0;      /* allocated capacity */

/* Grow the cache array when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Ensure that the cache holds at least 'target' terms */
static void ensure_cache(idx_t target)
{
    if (cache_len >= (size_t)target)
        return;   /* already enough */

    while (cache_len < (size_t)target) {
        seq_t p;
        if (cache_len == 0)                      /* first term */
            p = 5;
        else
            p = cache[cache_len - 1] + 2;        /* start after last found */

        /* Find the next qualifying prime */
        while (true) {
            /* Skip obvious composites */
            if (p > 5 && p % 5 == 0) {
                p += 2;
                continue;
            }
            if (!isPrimeGeneric(p)) {
                p += 2;
                continue;
            }

            seq_t two_p_plus_9  = p * 2 + 9;
            seq_t nine_p_plus_2 = p * 9 + 2;

            if (isPrimeGeneric(two_p_plus_9) && isPrimeGeneric(nine_p_plus_2)) {
                /* Store the term */
                if (cache_len == cache_cap)
                    grow_cache();

                cache[cache_len++] = p;
                break;   /* move to next term */
            }
            p += 2;  /* try next odd candidate */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but safe */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
