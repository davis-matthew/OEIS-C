/*
 * A094403  a(1) = 1; a(n) = (sum of previous terms)^n mod n.
 * https://oeis.org/A094403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum_{k=1}^{n-1} a(k))^n mod n; a(1) = 1
 * Range:     u8 n <= 38, u16 n <= 833, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* a[0] corresponds to a(1) */
    static size_t cap = 0;            /* current allocated capacity */
    static idx_t cached_n = 0;         /* largest n for which a is computed */
    static seq_t cum_sum = 0;          /* sum of a(1)..a(cached_n) */

    /* Initialise with a(1) = 1 on first call */
    if (cached_n == 0) {
        size_t init_cap = 64;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);           /* allocation failure */
        cap = init_cap;
        cache[0] = (seq_t)1;           /* a(1) */
        cum_sum = (seq_t)1;
        cached_n = 1;
    }

    if (n <= cached_n) {
        return cache[n-1];
    }

    /* Extend cache to at least n entries */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap*2 : 64;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms from cached_n+1 up to n */
    for (idx_t m = cached_n + 1; m <= n; ++m) {
        seq_t mod = (seq_t)m;
        /* (sum of previous terms) ^ m  mod m */
        seq_t val = mod_pow(cum_sum, (uint64_t)m, mod);
        cache[m-1] = val;
        cum_sum += val;
    }
    cached_n = n;
    return cache[n-1];
}
