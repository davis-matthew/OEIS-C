/*
 * A011663  A binary m-sequence: expansion of reciprocal of x^5+x^4+x^3+x+1.
 * https://oeis.org/A011663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) XOR a(n-3) XOR a(n-4) XOR a(n-5) (mod 2), with the
 *            initial values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1. The
 *            function caches computed terms for repeated calls
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of values currently stored */

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* On first call allocate space for the initial 5 terms. */
    if (cache_len == 0) {
        /* Ensure at least space for 5 elements. */
        size_t sz = n + 1;
        if (sz < 5) sz = 5;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory – caller handles */
        /* Initialise the first 5 terms. */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache_len = 5;
    }

    /* If n is already computed, return it. */
    if ((idx_t)cache_len > n) {
        return cache[n];
    }

    /* Grow the cache to accommodate up to n (inclusive). */
    size_t new_size = n + 1;
    if (new_size > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory – caller handles */
        cache = tmp;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] ^ cache[i-3] ^ cache[i-4] ^ cache[i-5];
    }
    cache_len = n + 1;    /* all terms up to n are now valid */

    return cache[n];
}
