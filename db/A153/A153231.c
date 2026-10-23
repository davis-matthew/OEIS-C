/*
 * A153231  a(n) = 2^n * binomial(3n,n)/(2n+1).
 * https://oeis.org/A153231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * binomial(3*n, n) / (2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t highest = 0;   /* highest index already computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;                     /* a(0) = 1 */
        highest = 0;
    }

    /* already computed? */
    if (n <= highest)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until large enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache_cap = new_cap;
    }

    /* compute terms from highest+1 up to n using the recurrence */
    for (idx_t i = highest + 1; i <= n; ++i) {
        /* numerator = 3 * (3*i - 1) * (3*i - 2) */
        seq_t num = (seq_t)3;
        num *= (seq_t)(3 * i - 1);
        num *= (seq_t)(3 * i - 2);

        /* denominator = i * (2*i + 1) */
        seq_t den = (seq_t)i;
        den *= (seq_t)(2 * i + 1);

        /* reduce fraction num/den first */
        seq_t g = gcd(num, den);
        if (g != 0) {
            num /= g;
            den /= g;
        }

        /* reduce denominator with previous term */
        seq_t prev = cache[i - 1];
        g = gcd(prev, den);
        if (g != 0) {
            prev /= g;
            den /= g;            /* den should now be 1 */
        }

        /* compute a(i) */
        cache[i] = prev * num;
    }

    highest = n;
    return cache[n];
}
