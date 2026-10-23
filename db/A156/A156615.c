/*
 * A156615  a(1)=2, a(n+1) is the smallest prime > n*final digit of a(n).
 * https://oeis.org/A156615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156615: 
   a(1)=2, a(n+1)=prime(p) where p is the smallest prime greater than
   n * (last digit of a(n)).  Indexing starts at 1. */

/* We rely on the utility function is_prime which tests primality of
   unsigned 64‑bit integers.  All intermediate values comfortably fit in
   64 bits for n up to at least 10 000. */
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* out of range, return 0 */

    /* Static cache to memoise computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;         /* number of terms stored */

    /* Ensure cache is large enough and initialize the first term. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * n);
        if (!cache)
            return 0;
        cache[0] = (seq_t)2;
        cache_len = 1;
    } else if (cache_len < n) {
        /* Grow cache to size n.  realloc with a NULL pointer acts like malloc. */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp)
            return 0;
        cache = tmp;
    }

    /* Compute any missing terms.  For any idx (0‑based) in [cache_len, n),
       a(idx+1) = smallest prime > idx * last_digit(a(idx)).  */
    for (idx_t idx = cache_len; idx < n; ++idx) {
        seq_t prev = cache[idx - 1];
        seq_t last_digit = prev % 10;          /* last decimal digit */
        seq_t m = ((seq_t)idx) * last_digit;   /* idx is the n for a(n+1) */
        idx_t candidate = (idx_t)m + 1;        /* start search after m */

        while (!is_prime(candidate))
            ++candidate;

        cache[idx] = (seq_t)candidate;
    }
    cache_len = n;    /* All terms up to n are now computed */

    return cache[n - 1];
}
