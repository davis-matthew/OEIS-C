/*
 * A137446  Positive numbers k such that k^2 +- (k-1) are primes.
 * https://oeis.org/A137446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that k^2-(k-1) and
 *            k^2+(k-1) are both prime. The function builds the sequence on
 *            demand, caching the results for future calls
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdbool.h>     /* bool */

/* Helpers supplied by the framework */
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Cached sequence data */
static seq_t *a_cache      = NULL;   /* array of found k's          */
static size_t a_cache_len  = 0;      /* number of stored values     */
static size_t a_cache_cap  = 0;      /* allocated capacity          */
static seq_t  a_next_k     = 2;      /* next candidate to test      */

seq_t a(idx_t n)
{
    /* OEIS offset is 1 – for n==0 we return 0 as a safe default. */
    if (n == 0)
        return 0;

    /* Grow the cache until it contains at least n elements. */
    while (a_cache_len < (size_t)n)
    {
        seq_t k = a_next_k;

        /* Compute p1 = k^2 - (k-1) and p2 = k^2 + (k-1) using
           seq_t arithmetic (works for built‑in unsigned types as well
           as for the arbitrary‑precision class). */
        seq_t kk  = k * k;
        seq_t p1  = kk - k + 1;   /* k^2 - (k-1) */
        seq_t p2  = kk + k - 1;   /* k^2 + (k-1) */

        if (is_prime_big(p1) && is_prime_big(p2))
            append_candidate(&a_cache, &a_cache_len, &a_cache_cap, k);

        ++a_next_k;
    }

    return a_cache[n - 1];
}
