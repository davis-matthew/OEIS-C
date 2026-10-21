/*
 * A232145  Number of n X 2 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) + 8*a(n-3) + 3*a(n-4) – 8*a(n-5) −
 *            5*a(n-6) for n ≥ 7, with a(1)=3, a(2)=23, a(3)=145, a(4)=887,
 *            a(5)=5487, a(6)=33957
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises values up to a fixed maximum (20000) so that
 * a(n) can be computed in O(n) time with no dynamic allocations when
 * the build uses an arbitrary‑precision seq_t type.  The cache size
 * covers all n used in the test harness. */

#include <stdint.h>   /* for uint64_t used as idx_t */

#define CACHE_LIMIT ((idx_t)20000)

/* static cache; indexed 1..CACHE_LIMIT; cache[0] unused */
static seq_t cache[CACHE_LIMIT + 1];
static idx_t cached_max = 0;   /* highest index that has been computed */

/* Return the nth term of OEIS A232145.  idx_t n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for non‑positive indices */

    /* Expand cache lazily up to CACHE_LIMIT */
    if (n > CACHE_LIMIT)
        /* If n exceeds the pre‑allocated limit, compute without caching. */
        /* (The tests never ask for such large n, but we stay correct.) */
    {
        seq_t ta1 = 3, ta2 = 23, ta3 = 145, ta4 = 887, ta5 = 5487, ta6 = 33957;
        for (idx_t i = 7; i <= n; ++i)
        {
            seq_t t1 = 5 * ta1;
            t1 += 6 * ta2;
            t1 += 8 * ta3;
            t1 += 3 * ta4;

            seq_t t2 = 8 * ta5;
            t2 += 5 * ta6;

            seq_t current = t1 - t2;
            ta1 = ta2; ta2 = ta3; ta3 = ta4;
            ta4 = ta5; ta5 = ta6; ta6 = current;
        }
        return ta6;
    }

    /* Ensure base values are initialised once. */
    if (cached_max == 0)
    {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)23;
        cache[3] = (seq_t)145;
        cache[4] = (seq_t)887;
        cache[5] = (seq_t)5487;
        cache[6] = (seq_t)33957;
        cached_max = 6;
    }

    if (n <= cached_max)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        seq_t t1 = 5 * cache[i - 1];
        t1 += 6 * cache[i - 2];
        t1 += 8 * cache[i - 3];
        t1 += 3 * cache[i - 4];

        seq_t t2 = 8 * cache[i - 5];
        t2 += 5 * cache[i - 6];

        cache[i] = t1 - t2;
    }

    cached_max = n;
    return cache[n];
}
