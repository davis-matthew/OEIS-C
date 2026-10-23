/*
 * A123079  Twin primes of form 4k+1.
 * https://oeis.org/A123079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/*
 * Sequence: the larger member of each twin prime pair, i.e.
 * numbers p such that p and p-2 are prime and p ≡ 1 (mod 4).
 * This implementation caches the first up to CACHE_CAP terms
 * and generates additional ones on demand.
 */

#define CACHE_CAP 50000   /* sufficient for the expected range of calls */

/* Static cache: array of found terms. */
static seq_t cache[CACHE_CAP];
static size_t cached_len = 0;

/* Next candidate of the form 4k+1 to test. */
static seq_t last_checked = (seq_t)5;

/* Ensure that cache contains at least n terms (n ≤ CACHE_CAP). */
static void ensure_cache(idx_t n)
{
    while (cached_len < n)
    {
        if (is_prime_big(last_checked))
        {
            seq_t prev = last_checked - (seq_t)2;
            seq_t next = last_checked + (seq_t)2;
            if (is_prime_big(prev) || is_prime_big(next))
            {
                cache[cached_len++] = last_checked;
            }
        }
        last_checked += (seq_t)4;   /* next number ≡ 1 (mod 4) */
    }
}

/* Helper: compute a(n) directly without caching (used when n > CACHE_CAP). */
static seq_t compute_direct(idx_t n)
{
    size_t count = 0;
    seq_t candidate = (seq_t)5;

    while (count < n)
    {
        if (is_prime_big(candidate))
        {
            seq_t prev = candidate - (seq_t)2;
            seq_t next = candidate + (seq_t)2;
            if (is_prime_big(prev) || is_prime_big(next))
            {
                count++;
                if (count == n)
                    return candidate;
            }
        }
        candidate += (seq_t)4;
    }
    return 0; /* never reached */
}

/* Sequence entry point. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1, but safe */

    if (n <= CACHE_CAP)
    {
        ensure_cache(n);
        return cache[n - 1];
    }

    return compute_direct(n);
}
