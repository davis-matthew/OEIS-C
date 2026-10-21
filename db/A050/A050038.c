/*
 * A050038  a(n) = a(n-1) + a(m) for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A050038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 4 a(n) = a(n-1) + a(n-1-2^p) for n ≥
 *            4, where p = ⌊log₂(n-2)⌋ (the unique integer with 2^p < n-1 ≤
 *            2^(p+1))
 * Range:     u8 n <= 31, u16 n <= 194, u32 n <= 1703, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute floor(log2(x)) for x ≥ 1; returns 0 for x == 1. */
static idx_t floor_log2_idx(idx_t x)
{
    idx_t p = 0;
    /* x ≥ 1, shift until only the highest bit remains */
    while (x >>= 1)
        ++p;
    return p;
}

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  length = 0;   /* maximum index computed so far */

    if (n == 0)            /* undefined, but return 0 */
        return (seq_t)0;

    /* ensure cache big enough for index n */
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = (idx_t)(length + 1); i <= n; ++i) {
        if (i == 1 || i == 2)
            cache[i] = (seq_t)1;
        else if (i == 3)
            cache[i] = (seq_t)4;
        else {
            idx_t p = floor_log2_idx(i - 2);          /* 2^p < i-1 ≤ 2^(p+1) */
            idx_t power_of_two = ((idx_t)1 << p);     /* 2^p */
            idx_t m = (i - 1) - power_of_two;        /* n-1-2^p */
            cache[i] = cache[i - 1] + cache[m];
        }
    }

    length = (size_t)n;
    return cache[n];
}
