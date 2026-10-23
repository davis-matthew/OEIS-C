/*
 * A186563  Number of (n+2) X 6 0..2 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) – a(n-2) + 2*a(n-3) – 4*a(n‑4) + 2*a(n‑5) +
 *            2*a(n‑6) – 2*a(n‑7) for n > 12. Initial values: a(1)=6594,
 *            a(2)=1411, a(3)=3421, a(4)=8983, a(5)=11557, a(6)=12895,
 *            a(7)=14845, a(8)=18415, a(9)=25381, a(10)=40279, a(11)=63565,
 *            a(12)=102591
 * Range:     u8 none, u16 n <= 11, u32 n <= 36, u64 n <= 88, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; grows as needed (size is n+1) */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;          /* highest n that is cached */

    /* helper to initialise the cache with known first 12 terms */
    if (cache == NULL) {
        /* allocate room for up to 12 */
        size_t init_size = 13;            /* indices 0..12 inclusive */
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */

        /* fill with zeros first */
        for (size_t i = 0; i < init_size; ++i)
            cache[i] = (seq_t)0;

        /* known values */
        const seq_t init[13] = {
            0ULL,            /* unused a(0) */
            6594ULL, 1411ULL, 3421ULL, 8983ULL, 11557ULL,
            12895ULL, 14845ULL, 18415ULL, 25381ULL,
            40279ULL, 63565ULL, 102591ULL
        };
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init[i];

        cached_max = 12;
    }

    /* if the requested n is already cached, return it */
    if (n <= cached_max)
        return cache[n];

    /* need to grow the cache up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;

    /* compute values up to n */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        /* compute positive terms: 2*a(k-1) + 2*a(k-3) + 2*a(k-5) + 2*a(k-6) */
        seq_t pos = cache[k-1];
        pos += pos;                      /* 2*a(k-1) */

        seq_t x = cache[k-3];
        pos += x; pos += x;              /* + 2*a(k-3) */

        x = cache[k-5];
        pos += x; pos += x;              /* + 2*a(k-5) */

        x = cache[k-6];
        pos += x; pos += x;              /* + 2*a(k-6) */

        /* compute negative terms:  a(k-2) + 4*a(k-4) + 2*a(k-7) */
        seq_t neg = cache[k-2];

        x = cache[k-4];
        neg += x; neg += x; neg += x; neg += x;   /* + 4*a(k-4) */

        x = cache[k-7];
        neg += x; neg += x;              /* + 2*a(k-7) */

        cache[k] = pos - neg;
    }

    cached_max = n;
    return cache[n];
}
