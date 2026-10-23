/*
 * A141948  Primes congruent to 1 mod 27.
 * https://oeis.org/A141948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141948: primes ≡ 1 (mod 27). */
/* Computed by searching upward from the last found prime, caching results. */

#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(uint64_t) */
#include "../utils/grow_seq.h"         /* grow_seq(seq_t**,size_t*,size_t) */

/* Return the n-th prime ≡ 1 (mod 27).  n ≥ 1. */
seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache  = NULL;   /* array of seq_t values */
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* state kept between calls to avoid re-scanning */
    static uint64_t last_checked = 0;   /* last number that was examined */
    static uint64_t next_prime   = 0;   /* last prime found that ≡1 mod 27 */

    if (n == 0)                /* OEIS offset is 1; return 0 for out‑of‑range */
        return (seq_t)0;

    /* Return cached value if already available */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* If we have not found any prime yet, start searching from 2 */
    if (next_prime == 0) {
        last_checked = 1;
        next_prime   = 0;
    }

    /* Continue searching until we have at least n cached terms */
    while (cache_len < (size_t)n) {
        /* Look for the next prime ≡ 1 mod 27.  Increment until found. */
        do {
            ++last_checked;
            /* Simple weekly optimization: skip even numbers > 2 */
            if (last_checked > 2 && (last_checked & 1) == 0)
                continue;
        } while (!is_prime_small(last_checked) || (last_checked % 27ULL) != 1ULL);

        /* Found a suitable prime */
        next_prime = last_checked;
        /* Ensure enough capacity in the cache */
        grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = (seq_t)next_prime;
    }

    return cache[(size_t)n - 1];
}
