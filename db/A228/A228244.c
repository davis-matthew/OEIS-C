/*
 * A228244  Primes of the form k^2 + 17.
 * https://oeis.org/A228244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form k^2 + 17 (OEIS A228244)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 14, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Returns the nth prime of the form k^2 + 17, with a(1)=17. */
seq_t a(idx_t n)
{
    /* Static cache for computed primes and next quadratic index */
    static seq_t *cache = NULL;
    static size_t len     = 0;  /* number of cached terms */
    static size_t cap     = 0;  /* capacity of cache array */
    static idx_t  next_k  = 0;  /* next k to test (k starts at 0) */

    if (n == 0)
        return 0;               /* undefined for offset < 1, return 0 */

    /* Expand cache until it contains at least n terms */
    while (len < (size_t)n) {
        /* Compute k^2 + 17 using seq_t arithmetic to avoid overflow of idx_t */
        seq_t val = ((seq_t)next_k * next_k) + (seq_t)17;

        /* Keep only primes */
        if (is_prime_big(val))
            append_candidate(&cache, &len, &cap, val);

        next_k++;
    }

    return cache[n - 1];
}
