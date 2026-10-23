/*
 * A185562  1/4 the number of n X 4 0..3 arrays with no element equal both to
 *          the element above and to the element to its left.
 * https://oeis.org/A185562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 217*a(n-1) - 1371*a(n-2) + 2835*a(n-3) - 2025*a(n-4)
 *            Initial values: a(1)=64, a(2)=13500, a(3)=2842560,
 *            a(4)=598507920
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;          /* 1‑based cache, cache[0] unused */
    static idx_t cache_len = 0;          /* number of allocated slots */

    /* Expand the cache to hold terms up to n */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;           /* allocate indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure – return 0 (rare) */
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 1) cache[1] = 64ULL;
            else if (i == 2) cache[2] = 13500ULL;
            else if (i == 3) cache[3] = 2842560ULL;
            else if (i == 4) cache[4] = 598507920ULL;
            else
            {
                seq_t t1 = 217 * cache[i-1];
                seq_t t2 = 1371 * cache[i-2];
                seq_t t3 = 2835 * cache[i-3];
                seq_t t4 = 2025 * cache[i-4];
                seq_t val = t1;
                val += t3;
                val -= t2;
                val -= t4;
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
