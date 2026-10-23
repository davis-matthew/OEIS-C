/*
 * A121629  Finite sum involving signless Stirling numbers of the first kind
 *          and the Bell numbers. Appears in the process of normal ordering of
 *          n-th power of (a)^2*(a+*a), where a+ and a are boson creation and
 *          annihilation operators, respectively.
 * https://oeis.org/A121629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p=1..n+1} abs(stirling1(n+1,p))*2^(n-p+1)*bell(p-1)
 *            Recurrence (for n>=3): a(n) = (6*n-5)*a(n-1) -
 *            (2*n-3)*(6*n-7)*a(n-2) + 4*(2*n-3)*(n-2)^2*a(n-3) initial
 *            values: a(0)=1, a(1)=3, a(2)=16
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"   /* provides seq_t mul_small(seq_t x, unsigned int m) */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_max = 0;
static bool initialized = false;

/* Ensure cache has space for indices 0..n and initialize first three terms.  */
static void ensure_cache_size(idx_t n)
{
    if (cache_size >= n + 1)
        return;

    size_t newsize = n + 1;
    /* grow by 1.5x to reduce reallocs */
    if (newsize < 8)
        newsize = 8;
    while (newsize < n + 1)
        newsize = newsize * 3 / 2;

    seq_t *newcache = (seq_t *)malloc(newsize * sizeof(seq_t));
    if (!newcache) {
        /* allocation failed – abort to avoid undefined behaviour */
        abort();
    }
    /* copy old data */
    for (size_t i = 0; i < cache_size; ++i)
        newcache[i] = cache[i];
    /* zero the rest */
    for (size_t i = cache_size; i < newsize; ++i)
        newcache[i] = 0;

    if (cache)
        free(cache);
    cache = newcache;
    cache_size = newsize;

    if (!initialized) {
        /* set the initial values */
        if (cache_size > 0) cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)3;
        if (cache_size > 2) cache[2] = (seq_t)16;
        computed_max = 2;
        initialized = true;
    }
}

/* Main function producing a(n).  */
seq_t a(idx_t n)
{
    if (!initialized)
        ensure_cache_size(2);

    if (n <= computed_max)
        return cache[n];

    ensure_cache_size(n);

    /* compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* term1 = (6*i-5) * a(i-1) */
        seq_t term1 = mul_small(cache[i-1], (unsigned int)(6*i - 5));

        /* term2 = (2*i-3)*(6*i-7) * a(i-2) */
        unsigned int coeff2 = (2*i - 3) * (6*i - 7);
        seq_t term2 = mul_small(cache[i-2], coeff2);

        /* term3 = 4*(2*i-3)*(i-2)^2 * a(i-3) */
        unsigned int coeff3 = 4 * (2*i - 3) * (i-2) * (i-2);
        seq_t term3 = mul_small(cache[i-3], coeff3);

        /* a(i) = term1 + term3 - term2 */
        /* Since term1+term3 > term2 for all valid n, subtraction won't underflow. */
        seq_t tmp = term1 + term3;
        cache[i] = tmp - term2;
    }

    computed_max = n;
    return cache[n];
}
