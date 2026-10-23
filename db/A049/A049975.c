/*
 * A049975  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049975
   a(1) = 1, a(2) = a(3) = 3
   a(n) = (sum_{k=1}^{n-1} a(k)) + a(m)   for n ≥ 4
   where m = 2*n − 2 − 2^(p+1) and p is the unique integer
   such that 2^p < n−1 ≤ 2^(p+1) */

/* The implementation memoizes all computed values in a
   dynamically growing static array so that any order of
   calls is supported efficiently. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state shared across calls */
    static seq_t *cache      = NULL;   /* cached a(i) values */
    static size_t cap        = 0;      /* current allocation size */
    static idx_t  maxComputed = 0;     /* highest index computed so far */
    static seq_t cumulativeSum = 0;    /* sum of a(1)..a(maxComputed) */

    /* Ensure the cache can hold at least n elements */
    if (n > cap) {
        size_t newCap = cap ? cap << 1 : 4;
        while (newCap < (size_t)n)
            newCap <<= 1;
        cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        cap = newCap;
    }

    /* Compute missing values up to n */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else if (i == 2 || i == 3) {
            val = 3;
        } else {
            /* Determine p: the largest p such that 2^p < i-1 */
            idx_t target = i - 2;            /* i-1 > 1 for i ≥ 4 */
            idx_t pw = 1;
            while ((pw << 1) <= target)
                pw <<= 1;
            /* m = 2*i − 2 − 2^(p+1) = 2*i − 2 − (pw << 1) */
            idx_t m = (idx_t)((seq_t)2 * i - 2 - ((seq_t)pw << 1));
            /* m < i always holds, so cache[m-1] is available */
            val = cumulativeSum + cache[(size_t)(m - 1)];
        }
        cache[(size_t)(i - 1)] = val;
        cumulativeSum += val;
    }

    maxComputed = n;
    return cache[(size_t)(n - 1)];
}
