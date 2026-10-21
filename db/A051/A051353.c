/*
 * A051353  a(n) = a(n-1) + n^2 if n prime else a(n-1) - n, starting with a(0)
 *          = 0.
 * https://oeis.org/A051353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = a(n-1) + n*(A010051(n)*(n+1)-1) , a(0)=0 (A010051(n) = 1
 *            if n is prime, 0 otherwise)
 * Range:     i8 n <= 10, i16 n <= 72, i32 n <= 3708, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: primality test (trial division, sufficient for n ≤ 10000) */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;   /* number of terms stored (a[0]..a[cached_len-1]) */

    /* extend the cache up to n */
    if ((size_t)n + 1 > cached_len) {
        size_t new_len = (size_t)n + 1;

        /* reallocate the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)   /* allocation failed – abort with zero (should never happen) */
            abort();
        cache = tmp;

        /* compute new terms */
        for (size_t i = cached_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = 0;   /* a(0) = 0 */
            } else {
                seq_t prev = cache[i - 1];
                if (is_prime((idx_t)i)) {
                    cache[i] = prev + (seq_t)i * (seq_t)i;  /* add n² for primes */
                } else {
                    cache[i] = prev - (seq_t)i;            /* subtract n for composites */
                }
            }
        }
        cached_len = new_len;
    }
    return cache[(size_t)n];
}
