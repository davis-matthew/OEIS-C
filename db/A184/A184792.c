/*
 * A184792  Numbers k such that floor(k*r) is prime, where r = golden
 *          ratio=(1+sqrt(5))/2.
 * https://oeis.org/A184792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer k (1‑based) such that floor(k*φ) is prime,
   where φ = (1+√5)/2. The sequence is generated incrementally and
   cached so that calls in any order run in O(n) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>          /* for sqrtl, not used in integer version   */
#include "../utils/is_prime_big.h"  /* provides bool is_prime_big(seq_t) */

/* integer square root for __int128 positive numbers */
static inline unsigned long long isqrt_u128(__int128 v) {
    unsigned long long lo = 0, hi = (1ull << 32);   /* hi > sqrt(5*max_k^2) for k < 2^32 */
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* cache of discovered k values (1‑based indexing in the array) */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t   next_k     = 1;     /* next candidate k to test */

/* append a value to the cache, expanding the array if necessary */
static void cache_append(seq_t val) {
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return the nth term of sequence A184792 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* below offset */

    /* return cached value if already computed */
    if (cache && n <= cache_size)
        return cache[n - 1];

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        idx_t k = next_k;

        /* floor(k*phi) = floor((k + floor(k*sqrt(5))) / 2)   */
        __int128 prod = (__int128)5 * (__int128)k * (__int128)k;
        unsigned long long sq = isqrt_u128(prod);
        seq_t phi_val = ((seq_t)k + (seq_t)sq) / 2;

        if (is_prime_big(phi_val)) {
            cache_append((seq_t)k);
        }
        ++next_k;
    }
    return cache[n - 1];
}
