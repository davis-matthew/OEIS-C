/*
 * A097334  a(n) = Sum_{k=0..n} C(n-k, floor(k/2))*2^k.
 * https://oeis.org/A097334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=3; for n>=3, a(n)=a(n-1)+4*a(n-3)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache the computed values so that calls in any order are fast. */
static seq_t* cache = NULL;          /* array holding computed terms          */
static idx_t  cache_size = 0;        /* current allocated array length        */
static idx_t  max_computed = (idx_t)-1; /* highest index that has been computed */

seq_t a(idx_t n) {
    /* Initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 3;          /* we need at least indices 0,1,2 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure → 0   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)3;
        max_computed = 2;
    }

    /* If requested index is already in cache, just return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow cache if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        seq_t* tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + 4 * a(i-3) */
        cache[i] = cache[i-1] + (seq_t)4 * cache[i-3];
    }
    max_computed = n;
    return cache[n];
}
