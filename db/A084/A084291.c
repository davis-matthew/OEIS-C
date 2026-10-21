/*
 * A084291  Difference between consecutive primes arising after difference
 *          (d=2) between twin primes. In A001223, terms succeeding those that
 *          equal 2.
 * https://oeis.org/A084291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses prime gaps; a(n) is the gap following each twin‑prime gap (gap = 2). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache */
    static size_t size  = 0;   /* number of values already computed */

    /* static state for generating primes */
    static idx_t p_prev = 2;          /* previous prime */
    static idx_t p_cur  = 3;          /* current prime   */
    static idx_t p_next = 0;          /* next prime after p_cur */
    static bool  initialized = false;

    /* Initialise the prime state once */
    if (!initialized) {
        p_next      = next_prime(p_cur + 1);
        initialized = true;
    }

    /* Grow cache if needed */
    if (cap < n) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Generate more terms until we reach index n */
    while (size < n) {
        seq_t gap1 = (seq_t)(p_cur - p_prev); /* gap between p_prev and p_cur */
        seq_t gap2 = (seq_t)(p_next - p_cur); /* gap after p_cur */

        if (gap1 == (seq_t)2) {
            cache[size++] = gap2; /* store the following gap */
        }

        /* advance the sliding window of primes */
        p_prev = p_cur;
        p_cur  = p_next;
        p_next = next_prime(p_cur + 1);
    }

    return cache[n - 1];  /* a(1) corresponds to cache[0] */
}
