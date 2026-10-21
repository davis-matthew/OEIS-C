/*
 * A228019  Composite numbers whose sum of digits is a composite number.
 * https://oeis.org/A228019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, grow_seq, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228019: composite numbers whose digit sum is composite */
/* The implementation iteratively searches for the required numbers
   and memoises the results so that calls may arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"
#include "../utils/digit_sum.h"
#include "../utils/grow_seq.h"

/* cache for memoisation */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current allocated capacity */
static seq_t last_term = 0;      /* last term stored in the cache */

/* ensure the cache can store at least `min_cap` elements */
static void ensure_capacity(size_t min_cap)
{
    if (cache_cap < min_cap)
        grow_seq(&cache, &cache_cap, min_cap);
}

seq_t a(idx_t n)
{
    /* any n < 1 is outside the defined offset; return 0 as a safe default */
    if (n < 1)
        return (seq_t)0;

    /* grow the cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        seq_t cand = last_term;
        while (true)
        {
            cand = cand + (seq_t)1;                 /* next integer */

            /* skip 0 and 1 – they are not composite */
            if (cand < (seq_t)2)
                continue;

            /* skip primes */
            if (is_prime_big(cand))
                continue;

            /* evaluate the digit sum */
            seq_t sum = digit_sum(cand);

            /* a composite sum means >1 and not prime (sum>1 && !is_prime_small) */
            if (sum > (seq_t)1 && !is_prime_small(sum))
            {
                /* found the next required number */
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = cand;
                last_term = cand;
                break; /* outer cand loop */
            }
        }
    }

    return cache[(size_t)n - 1];
}
