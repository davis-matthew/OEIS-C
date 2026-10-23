/*
 * A176716  a(1) = 1; a(2*n) = prime(n)*a(n), a(2*n+1) = prime(n)*a(n) +
 *          a(n+1), where prime(n) is the n-th prime.
 * https://oeis.org/A176716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 147, u64 n <= 1151,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A176716
   a(1) = 1;
   a(2*n)   = prime(n) * a(n);
   a(2*n+1) = prime(n) * a(n) + a(n+1);
   Offset 1
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* static cache for computed values; index 0 unused */
static seq_t   *cache   = NULL;
static idx_t    cache_cap = 0;

/* Ensure cache is large enough for index n */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n) return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < n) new_cap <<= 1;

    seq_t *new_cache;
#if defined(__cplusplus)
    /* C++: use operator new[] to allocate; object types may have
       non-trivial default constructors.  They are all value-initialized
       so we can rely on the default constructor (zero or 0).  */
    new_cache = new seq_t[new_cap + 1];
    /* Initialize the new slots to zero (or default value) */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) new_cache[i] = 0;
    /* Copy old values to new buffer */
    for (idx_t i = 1; i <= cache_cap; ++i) new_cache[i] = cache[i];
    /* Delete old buffer */
    delete[] cache;
#else
    /* C: use realloc; seq_t is a plain integer type */
    new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    /* initialise the new entries */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) new_cache[i] = 0;
#endif
    /* keep index 0 as unused sentinel */
    new_cache[0] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    if (n == 1) {
        cache[1] = 1;
        return 1;
    }

    if ((n & 1) == 0) {            /* even: n = 2 * m */
        idx_t m = n >> 1;
        seq_t prime_m = get_prime(m);   /* prime(m) */
        cache[n] = prime_m * a(m);
        return cache[n];
    } else {                       /* odd: n = 2 * m + 1 */
        idx_t m = n >> 1;          /* floor(n/2) */
        seq_t prime_m = get_prime(m);  /* prime(m) */
        cache[n] = prime_m * a(m) + a(m + 1);
        return cache[n];
    }
}
