/*
 * A102339  Numbers k such that k*10^3 + 333 is prime.
 * https://oeis.org/A102339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer k such that k*10^3 + 333 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/resize_array.h"
#include "../utils/is_prime_uint64.h"

/* Cache of already discovered k values. */
static seq_t *cache = NULL;      /* array of seq_t */
static idx_t  cached = 0;        /* number of values currently stored   */
static idx_t  cap    = 0;        /* allocated capacity                  */
static idx_t  next_k = 1;        /* next candidate k to test            */

/* Extend the cache to hold at least N elements.  Uses the provided
   resize_array helper to reallocate the array when needed. */
static void ensure_cache(idx_t N)
{
    if (cached >= N)
        return;

    /* Grow capacity until it can hold N elements.  Double each time. */
    if (cap < N) {
        cap = cap ? cap << 1 : 2;
        while (cap < N)
            cap <<= 1;
    }

    /* Resize the array with the helper.  It returns a pointer suitable
       for the new size; if it returns NULL, we simply leave the cache
       unchanged (the caller will handle the failure at its own risk). */
    seq_t *tmp = resize_array(cache, cached, cap);
    if (tmp == NULL) {
        /* Allocation failure – leave cache as is. */
        return;
    }
    cache = tmp;

    /* Find new k values until we have N entries.  The primality test
       operates on 64‑bit values, which is safe for the range of k
       required for the expected sequence lengths. */
    while (cached < N) {
        uint64_t val = 1000ULL * (uint64_t)next_k + 333ULL;
        if (is_prime_uint64(val)) {
            cache[cached++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Required public function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n = 0, but safe to return 0 */

    ensure_cache(n);
    return cache[n - 1];
}
