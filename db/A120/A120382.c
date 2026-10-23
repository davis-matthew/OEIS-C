/*
 * A120382  Even numbers k such that 3*k-1 and 3*k+1 are not prime.
 * https://oeis.org/A120382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A060461(n) Generating even numbers k such that 3*k-1
 *            and 3*k+1 are both composite. The implementation scans even
 *            integers, tests each pair for primality using the provided
 *            is_prime_big helper, and caches the found terms so that
 *            subsequent calls with any n are answered in O(1) after the n...
 * Range:     u8 n <= 6, u16 n <= 4338, u32 n <= 6344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache   = NULL;   /* cache of found terms */
static idx_t  n_cache = 0;      /* number of terms stored */
static idx_t  cap     = 0;      /* capacity of cache array */
static idx_t  next    = 2;      /* next even integer to test */

/* append a value to the cache, expanding the array if necessary */
static void append_cache(seq_t val)
{
    if (n_cache >= cap) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed – silence; caller will
                                        not reach this point for supported n */
        cache = tmp;
        cap = new_cap;
    }
    cache[n_cache++] = val;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache. */
    while (n_cache < n) {
        idx_t k = next;
        next += 2;                     /* next even candidate */

        seq_t t1 = (seq_t)k * (seq_t)3 - (seq_t)1; /* 3*k-1 */
        seq_t t2 = (seq_t)k * (seq_t)3 + (seq_t)1; /* 3*k+1 */

        if (!is_prime_big(t1) && !is_prime_big(t2)) {
            append_cache((seq_t)k);
        }
    }
    return cache[n - 1];
}
