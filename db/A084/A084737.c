/*
 * A084737  Beginning with 1, numbers such that (a(n+2)-a(n+1))/(a(n+1)-a(n))
 *          = prime(n).
 * https://oeis.org/A084737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084737 – a(n) = 1 + Sum_{k=1}^{n-2} Primorial(k)
   (the successive differences are primorials: 1,2,6,30,210,...).
   a(1)=1, a(2)=2, a(n+1)=a(n)+d(n) with d(1)=1,
   d(k+1)=prime(k)*d(k) (prime(1)=2).  */

/*  The implementation uses a small on‑demand prime generator
    and memoises the computed terms in a static array.     */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* Sequence function.  Memoisation guarantees that repeated calls
   are fast regardless of the order of the indices.             */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Static state for memoisation */
    static seq_t *cache       = NULL;
    static size_t cache_size  = 0;
    static size_t cache_cap   = 0;
    static seq_t prod          = 0;    /* current difference d(k)   */
    static idx_t prime_idx    = 0;    /* next prime index to use   */

    /* initialise on first call */
    if (cache_size == 0) {
        cache_cap   = 4;
        cache       = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;         /* a(1) */
        cache[1] = (seq_t)2;         /* a(2) */
        prod      = (seq_t)1;        /* d(1) = 1 */
        prime_idx = 1;               /* prime(1) will be used next */
        cache_size = 2;
    }

    /* grow cache if needed */
    if ((size_t)n > cache_cap) {
        while (cache_cap < (size_t)n) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* already computed? */
    if (n <= cache_size) return cache[(size_t)n - 1];

    /* compute missing terms iteratively */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        uint32_t p = get_prime(prime_idx);
        seq_t next_diff = prod * (seq_t)p;
        seq_t new_val   = cache[i - 1] + next_diff;
        prod           = next_diff;
        ++prime_idx;
        cache[i] = new_val;
    }

    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
