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
 * Validated: standard, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   ipow_i32, mem
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
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

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
