/*
 * A061393  Number of appearances of n in sequence defined by b(k) =
 *          b(floor(k/3)) + b(ceiling(k/3)) with b(0)=0 and b(1)=1, i.e., in
 *          A061392.
 * https://oeis.org/A061393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(2n) = 3*a(n) - 2; a(2n+1) = 2
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9653, u64 n <= 9653,
 *            u128 n <= 9653, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A061393: number of appearances of n in the sequence defined by
   b(k) = b(floor(k/3)) + b(ceil(k/3)), b(0)=0, b(1)=1.
   The recurrence a(2n)=3*a(n)-2, a(2n+1)=2 holds for n>0. */

#include <stdlib.h>

/* Forward declaration required by the problem statement */
seq_t a(idx_t n);

/* Internal recursive routine with memoisation */
static seq_t a_rec(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* allocate cache on first use or expand if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size ? cache_size : 128;
        while (new_size <= n)
            new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* out of memory – abort silently (unlikely in tests) */
            abort();
        }
        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* return cached value if already computed */
    if (cache[n] != (seq_t)0)
        return cache[n];

    seq_t result;
    if ((n & (idx_t)1) == (idx_t)1) {   /* odd index 2m+1 */
        result = (seq_t)2;
    } else {                            /* even index 2m */
        result = (seq_t)3 * a_rec(n >> 1) - (seq_t)2;
    }

    cache[n] = result;
    return result;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
