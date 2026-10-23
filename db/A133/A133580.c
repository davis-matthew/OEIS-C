/*
 * A133580  a(0)=a(1)=1; for n>1, a(n) = 2*a(n-1) + 1 if a(n-1) and n are
 *          coprime, otherwise a(n) = a(n-1)/gcd(a(n-1),n).
 * https://oeis.org/A133580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 48, u64 n <= 99, u128 n <= 207,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133580:  a(0)=a(1)=1; for n>1,
   a(n) = 2*a(n-1)+1 if gcd(a(n-1),n)=1,
   otherwise a(n) = a(n-1)/gcd(a(n-1),n). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)-1;

    /* initialise cache with first two terms */
    if (!cache) {
        cache_cap = 4;                                /* start with small capacity */
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t)); /* cast works in C and C++ */
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 1;
        last_computed = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t)); /* cast works in C and C++ */
        if (!tmp) abort();
        /* initialise new slots */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return it */
    if (n <= (idx_t)last_computed) return cache[n];

    /* compute from last_computed+1 to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t g    = gcd(prev, (seq_t)i);
        if (g == 1) cache[i] = prev * (seq_t)2 + (seq_t)1;
        else       cache[i] = prev / g;
    }
    last_computed = n;
    return cache[n];
}
