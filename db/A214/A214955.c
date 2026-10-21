/*
 * A214955  Number of solid standard Young tableaux of shape [[n,n-1],[1]].
 * https://oeis.org/A214955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*C(2*n,n)/(n+1) equivalently a(1)=1 and for n>1:
 *            a(n)=2*(2*n-1)^2/((n+1)*(2*n-3))*a(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* no defined value for n==0 */

    static seq_t *cache = NULL;   /* 1‑indexed cache of results */
    static idx_t cached_max = 0;  /* highest n that has been computed */
    static size_t cache_cap = 0;  /* current allocated size */

    /* Allocate initial cache */
    if (cache == NULL) {
        cache_cap = 4;            /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        cache_cap = new_cap;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* initialise a(1) if not yet done */
    if (cached_max < 1) {
        cache[1] = 1;
        cached_max = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i == 1) {                     /* already handled */
            continue;
        }
        seq_t prev = cache[i - 1];

        /* compute numerator = 2*(2*i-1)^2 and denominator = (i+1)*(2*i-3) */
        seq_t two_i_minus_1 = (seq_t)(2 * i - 1);
        seq_t numerator = 2 * two_i_minus_1 * two_i_minus_1;
        seq_t denominator = (seq_t)(i + 1) * (seq_t)(2 * i - 3);

        /* Reduce to avoid intermediate overflow: a = prev * num / den */
        seq_t g1 = gcd(prev, denominator);
        seq_t prev_reduced = prev / g1;
        seq_t den_reduced  = denominator / g1;

        seq_t g2 = gcd(numerator, den_reduced);
        seq_t num_reduced = numerator / g2;
        seq_t den_final   = den_reduced / g2;   /* should be 1 but keep for safety */

        seq_t temp = prev_reduced * num_reduced;
        cache[i] = temp / den_final;
    }
    cached_max = n;

    return cache[n];
}
