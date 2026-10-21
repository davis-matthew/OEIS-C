/*
 * A233710  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 14.
 * https://oeis.org/A233710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Recurrence of order 8 (empirical, from the OEIS entry)
 *
 * a(1) = 96
 * a(2) = 444
 * a(3) = 1992
 * a(4) = 9100
 * a(5) = 41256
 * a(6) = 187780
 * a(7) = 853104
 * a(8) = 3879380
 *
 * For n ≥ 9:
 * a(n) = 24*a(n‑2) + 14*a(n‑3) – 129*a(n‑4) – 68*a(n‑5)
 *        + 228*a(n‑6) + 64*a(n‑7) – 128*a(n‑8)
 *
 * The recurrence is used to generate all terms in O(n) time with a
 * dynamic cache, so that any call to a(idx_t n) is answered quickly.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of valid entries (size of cache) */

    /* Helper to initialise cache with initial eight terms */
    if (cache == NULL)
    {
        size_t init_size = 9;        /* indices 0..8, index 0 unused */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();        /* safety: terminate on malloc failure */
        cache_len = init_size;

        cache[0] = 0;               /* unused, makes indices 1..8 valid */
        cache[1] = 96;
        cache[2] = 444;
        cache[3] = 1992;
        cache[4] = 9100;
        cache[5] = 41256;
        cache[6] = 187780;
        cache[7] = 853104;
        cache[8] = 3879380;
    }

    if (n == 0)        /* offset starts at 1 – value for 0 is undefined */
        return 0;

    /* If the requested term already exists in the cache, return it */
    if (n < cache_len)
        return cache[n];

    /* If we need terms beyond what we have, extend the cache and compute them */
    if (n >= cache_len)
    {
        /* Allocate space for the new terms */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                /* safety: terminate on realloc failure */
        cache = tmp;
        /* Initialise new memory to 0 (optional, but safe) */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i <= 8) {
                /* Should not happen because cache already contains 1..8 */
                continue;
            }
            /* Recurrence for i ≥ 9 */
            seq_t pos = cache[i-2] * 24;    /* 24*a(i‑2)   */
            pos += cache[i-3] * 14;         /* +14*a(i‑3)  */
            pos += cache[i-6] * 228;        /* +228*a(i‑6) */
            pos += cache[i-7] * 64;         /* +64*a(i‑7)  */

            seq_t neg = cache[i-4] * 129;    /* 129*a(i‑4)  */
            neg += cache[i-5] * 68;         /* +68*a(i‑5)  */
            neg += cache[i-8] * 128;        /* +128*a(i‑8) */

            cache[i] = pos - neg;           /* a(i) = pos – neg */
        }
        cache_len = new_len;
    }

    return cache[n];
}
