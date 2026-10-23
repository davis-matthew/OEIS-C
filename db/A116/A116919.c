/*
 * A116919  a(0)=1. a(n) = A006530(a(n-1)) + n.
 * https://oeis.org/A116919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1. a(n) = A006530(a(n-1)) + n with A006530(x) = largest
 *            prime factor of x
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the largest prime factor of x.
   For x <= 1 the function returns 1. */
static seq_t largest_prime_factor(seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)1;

    seq_t temp = x;
    seq_t biggest = (seq_t)1;

    /* Handle factor 2 */
    seq_t two = (seq_t)2;
    if (temp % two == 0) {
        biggest = two;
        while (temp % two == 0) temp /= two;
    }

    /* Test odd factors */
    seq_t p = (seq_t)3;
    for (;;) {
        seq_t p2 = p * p;
        if (p2 > temp) break;
        if (temp % p == 0) {
            biggest = p;
            while (temp % p == 0) temp /= p;
        }
        p += (seq_t)2;
    }

    /* Remaining prime > sqrt(original) */
    if (temp > (seq_t)1) {
        if (temp > biggest) biggest = temp;
    }

    return biggest;
}

/* Memoised sequence a(n). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with a(0) = 1 */
    if (cache_len == (idx_t)0) {
        cache_len = (idx_t)1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
    }

    /* Extend cache up to n */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + (idx_t)1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t prev = cache[i - (idx_t)1];
            seq_t lp   = largest_prime_factor(prev);
            cache[i] = lp + (seq_t)i;
        }
        cache_len = new_len;
    }

    return cache[n];
}
