/*
 * A237504  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock differing from its
 *          horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 208*a(n-4) + 1448*a(n-6) + 2500*a(n-8) + 528*a(n-10) -
 *            624*a(n-12) with initial values: a(1)=81, a(2)=328, a(3)=1342,
 *            a(4)=5622, a(5)=23545, a(6)=97700, a(7)=406008, a(8)=1690160,
 *            a(9)=7044600, a(10)=29290848, a(11)=121938192, a(12)=507239832
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[13] = {
    0ULL,                 /* unused index 0 */
    81ULL, 328ULL, 1342ULL, 5622ULL, 23545ULL, 97700ULL,
    406008ULL, 1690160ULL, 7044600ULL, 29290848ULL,
    121938192ULL, 507239832ULL
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n=0 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of slots stored (index 0..cache_size-1) */

    /* Allocate or grow cache to hold indices up to n (inclusive) */
    if (cache_size <= n) {
        idx_t new_size = n + 1;  /* need room for index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {            /* allocation failure – terminate program */
            exit(1);
        }
        cache = tmp;
        /* zero‑fill newly allocated slots (old contents remain valid) */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Ensure the first 12 terms are defined */
    for (idx_t i = 1; i <= 12 && i <= n; ++i)
        if (cache[i] == 0)
            cache[i] = INIT[i];

    /* Compute missing entries using the recurrence */
    for (idx_t i = 13; i <= n; ++i) {
        if (cache[i] != 0)   /* already computed */
            continue;

        /* recurrence calculation */
        cache[i] =
            208ULL * cache[i - 4] +
            1448ULL * cache[i - 6] +
            2500ULL * cache[i - 8] +
            528ULL * cache[i -10] -
            624ULL * cache[i -12];
    }

    return cache[n];
}
