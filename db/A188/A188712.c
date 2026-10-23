/*
 * A188712  Number of 8Xn binary arrays without the pattern 0 0 diagonally or
 *          vertically.
 * https://oeis.org/A188712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 55*a(n-1) -933*a(n-2) +7094*a(n-3) -27502*a(n-4)
 *            +57434*a(n-5) * -65257*a(n-6) +38535*a(n-7) -10341*a(n-8)
 *            +900*a(n-9) * with initial values: * a(1)=55, a(2)=1681,
 *            a(3)=54049, a(4)=1729531, a(5)=55337580, * a(6)=1769780565,
 *            a(7)=56585607231, a(8)=1808994829500, * a(9)=57829148027161,
 *            a(10)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[11] = {
    (seq_t)55,
    (seq_t)1681,
    (seq_t)54049,
    (seq_t)1729531,
    (seq_t)55337580,
    (seq_t)1769780565,
    (seq_t)56585607231ULL,
    (seq_t)1808994829500ULL,
    (seq_t)57829148027161ULL,
    (seq_t)1848624216954643ULL,
    (seq_t)59094629276262316ULL
};

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;   /* array of a(1)...a(current_max) */
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    /* Ensure cache is initialized and large enough */
    if (cache == NULL) {
        cache_cap = (n > 11) ? n : 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure, unlikely in tests */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_len = 11;
    } else if (n > cache_cap) {
        cache_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* allocation failure, unlikely in tests */
        cache = new_cache;
    }

    /* Return early if the value is already cached */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms using the recurrence, storing each */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i is 1‑based index for a(i) */
        seq_t new_term = cache[i - 2] * (seq_t)55
                       - cache[i - 3] * (seq_t)933
                       + cache[i - 4] * (seq_t)7094
                       - cache[i - 5] * (seq_t)27502
                       + cache[i - 6] * (seq_t)57434
                       - cache[i - 7] * (seq_t)65257
                       + cache[i - 8] * (seq_t)38535
                       - cache[i - 9] * (seq_t)10341
                       + cache[i -10] * (seq_t)900;
        cache[i - 1] = new_term;
    }

    cache_len = n;
    return cache[n - 1];
}
