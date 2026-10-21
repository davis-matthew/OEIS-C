/*
 * A055402  Least number represented as the sum of n cubes with greedy
 *          algorithm.
 * https://oeis.org/A055402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + ceil(sqrt(a(n-1)/3 + 1/4) - 1/2)^3 for
 *            n >= 2
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 16, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_seq_array, floor_sqrt, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the fact that
 *   ceil(sqrt(a/3 + 1/4) - 1/2) is the smallest integer k such that
 *   3*k*(k+1) >= a.
 * The increment is then k^3.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"      /* seq_t floor_sqrt(seq_t) */
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t*) */

static seq_t _k_from_a(seq_t a)
{
    seq_t low  = 0;
    seq_t high = floor_sqrt(a);
    high = high + (seq_t)1;

    /* Ensure high satisfies 3*high*(high+1) >= a */
    while (true) {
        seq_t t = high * (high + (seq_t)1);
        t = t * (seq_t)3;
        if (t >= a)
            break;
        /* double high to quickly reach the bound */
        high += high;
    }

    /* Binary search between low and high */
    while (low + (seq_t)1 < high) {
        seq_t mid = (low + high) >> 1;   /* integer division by 2 */
        seq_t t = mid * (mid + (seq_t)1);
        t = t * (seq_t)3;
        if (t >= a)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Lazily allocated static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   cap   = 0;

    if (n == 0)        /* sequence is defined only for n >= 1 */
        return 0;

    /* Allocate or grow cache to hold values up to n (index 1..n) */
    if (!cache || cap < n + (idx_t)1) {
        if (cache) free_seq_array(cache);
        cache = alloc_seq_array((size_t)(n + (idx_t)1));
        cap   = n + (idx_t)1;
    }

    if (cache[n] != (seq_t)0)      /* already computed */
        return cache[n];

    /* Base case: a(1) = 1 */
    cache[1] = (seq_t)1;
    if (n == 1)
        return cache[1];

    /* Compute values sequentially up to n */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t k    = _k_from_a(prev);
        seq_t k2   = k * k;
        seq_t k3   = k2 * k;
        cache[i]   = prev + k3;
    }
    return cache[n];
}
