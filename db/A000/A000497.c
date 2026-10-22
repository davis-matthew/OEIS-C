/*
 * A000497  S2(j,2j+2) where S2(n,k) is a 2-associated Stirling number of the
 *          second kind.
 * https://oeis.org/A000497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1)*(n+1)*(2n+1)^2/((2n-1)*(n-1)), a(1) = 1
 *            (equivalently a(n) = n*(n+1)*(2n+1)*(2n+1)!!/18)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   a497_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a497_gcd.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_filled = 0; /* cache[1..cache_filled] valid */

seq_t a(idx_t n) {
    if (n <= 1) return 1;

    if (cache == NULL) {
        cache_cap = 1024;
        while (cache_cap <= n) cache_cap *= 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_cap; i++) cache[i] = 0;
        cache[1] = 1;
        cache_filled = 1;
    } else if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *nc = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < new_cap; i++) nc[i] = 0;
        for (idx_t i = 0; i <= cache_filled; i++) nc[i] = cache[i];
        free(cache);
        cache = nc;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_filled + 1; i <= n; i++) {
        /* ratio p/q = (i+1)(2i+1)^2 / ((2i-1)(i-1)), reduced to lowest terms */
        unsigned long long n1 = (unsigned long long)i + 1;
        unsigned long long n2 = 2ULL * i + 1;
        unsigned long long n3 = n2;
        unsigned long long d1 = 2ULL * i - 1;
        unsigned long long d2 = (unsigned long long)i - 1;
        unsigned long long g;

        g = a497_gcd(n1, d1); n1 /= g; d1 /= g;
        g = a497_gcd(n2, d1); n2 /= g; d1 /= g;
        g = a497_gcd(n3, d1); n3 /= g; d1 /= g;
        g = a497_gcd(n1, d2); n1 /= g; d2 /= g;
        g = a497_gcd(n2, d2); n2 /= g; d2 /= g;
        g = a497_gcd(n3, d2); n3 /= g; d2 /= g;

        /* q = d1*d2 is coprime to p, so q divides the previous term */
        seq_t q = (seq_t)d1 * (seq_t)d2;
        seq_t v = cache[i - 1] / q;
        v *= (seq_t)n1;
        v *= (seq_t)n2;
        v *= (seq_t)n3;
        cache[i] = v;
    }
    if (n > cache_filled) cache_filled = n;

    return cache[n];
}
