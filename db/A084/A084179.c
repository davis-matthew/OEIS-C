/*
 * A084179  Expansion of the g.f. x/((1+2x)(1-x-x^2)).
 * https://oeis.org/A084179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) + 3*a(n-2) + 2*a(n-3) for n > 2 with initial
 *            values: a(0)=0, a(1)=1, a(2)=-1
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache to store already computed terms */
static seq_t *cache = NULL;           /* array of terms */
static idx_t cache_capacity = 0;      /* allocated size */
static idx_t cache_size = 0;          /* number of terms currently computed */

/* ensure that cache has terms up to index n */
static void compute_up_to(idx_t n)
{
    if (n < cache_size)               /* already have it */
        return;

    /* initial allocation if necessary */
    if (cache_capacity == 0) {
        cache_capacity = 4;           /* start small, grow as needed */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    }

    /* grow the allocation to store at least n+1 elements */
    if (cache_capacity <= n) {
        while (cache_capacity <= n) cache_capacity <<= 1;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
    }

    /* initialise the first terms if this is the first run */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache_size = 3;
    }

    /* compute the remaining terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = -(cache[i-1])
                 + (seq_t)3 * cache[i-2]
                 + (seq_t)2 * cache[i-3];
    }

    /* update the number of computed terms */
    if (cache_size < n + 1)
        cache_size = n + 1;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
