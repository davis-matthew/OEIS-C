/*
 * A155171  Numbers p such that if q = p+1 then (a = q^2-p^2, b = 2*p*q, c =
 *          q^2 + p^2) is a primitive Pythagorean triple with s-1 and s+1
 *          primes, where s = a+b+c.
 * https://oeis.org/A155171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 16, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function computes A155171: the numbers p for which
 * q = p+1 yields a primitive Pythagorean triple (a,b,c) = (q²‑p², 2pq, q²+p²)
 * such that s−1 and s+1 are prime, where s = a+b+c.
 * In terms of p, the condition is that
 *     s = 4p² + 6p + 2
 * and both s-1 and s+1 are prime.
 * The sequence is generated on demand and cached for efficient repeated
 * calls.  Primality tests are performed with the provided is_prime_big()
 * routine.
 *
 * Since the sequence terms are strictly increasing, the algorithm
 * remembers the last tested p and expands the cache only when a
 * larger index is requested.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    /* Static cache for already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* capacity of the array */
    static idx_t last_p_checked = 0; /* last p value that was tested */

    /* Expand cache until it contains at least n terms */
    while (cache_len < n) {
        /* Test next p value */
        ++last_p_checked;  /* p to test */
        seq_t p = (seq_t)last_p_checked;

        /* Compute s = 4p² + 6p + 2 in seq_t */
        seq_t s = 4 * p * p + 6 * p + 2;

        /* Check primality of s-1 and s+1 */
        if (is_prime_big(s - 1) && is_prime_big(s + 1)) {
            append_candidate(&cache, &cache_len, &cache_cap, p);
        }
    }

    return cache[n - 1];
}
