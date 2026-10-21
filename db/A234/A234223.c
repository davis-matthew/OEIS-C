/*
 * A234223  Number of (n+1) X (4+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the empirical linear recurrence a(n) = 10*a(n-1) +
 *            16*a(n-2) -460*a(n-3) +789*a(n-4) +7170*a(n-5) -23944*a(n-6)
 *            -39920*a(n-7) +246076*a(n-8) -38440*a(n-9) -1115136*a(n-10)
 *            +1151040*a(n-11) +2005056*a(n-12) -3657600*a(n-13)
 *            -331776*a(n-14) +3456000*a(n-15) -1658880*a(n-16) with initia...
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms, index 0 unused */
    static const seq_t init[20] = {
        0ULL,
        7696ULL,
        20724ULL,
        56936ULL,
        167688ULL,
        503272ULL,
        1597896ULL,
        5162984ULL,
        17479656ULL,
        60126376ULL,
        215032104ULL,
        779519336ULL,
        2918196648ULL,
        11040350632ULL,
        42883189416ULL,
        167792882024ULL,
        670921667496ULL,
        2694321027496ULL,
        11019119460264ULL,
        45144426968936ULL
    };

    static seq_t *cache = NULL;          /* memoized sequence values */
    static size_t cache_cap = 0;          /* allocated capacity (elements) */
    static idx_t cache_max = 0;           /* highest index with a valid value */

    if (n == 0) return 0;                /* safety, offset starts at 1 */

    /* Ensure the cache has room for index n */
    if (n + 1 > cache_cap) {
        size_t needed = n + 1;
        size_t new_cap = needed > 2 * cache_cap ? needed : 2 * cache_cap;
        if (new_cap == 0) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill initial terms that are not yet in the cache */
    if (cache_max < 19) {
        idx_t limit = cache_cap > 0 ? cache_cap - 1 : 0;
        for (idx_t i = cache_max + 1; i <= 19 && i <= limit; ++i)
            cache[i] = init[i];
        /* Update cache_max to the highest index now stored */
        if (limit >= 19) cache_max = 19;
        else             cache_max = limit;
    }

    /* Return value if already computed */
    if (n <= cache_max) return cache[n];

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += 10 * cache[i-1];
        pos += 16 * cache[i-2];
        pos += 789 * cache[i-4];
        pos += 7170 * cache[i-5];
        pos += 246076 * cache[i-8];
        pos += 1151040 * cache[i-11];
        pos += 2005056 * cache[i-12];
        pos += 3456000 * cache[i-15];

        neg += 460 * cache[i-3];
        neg += 23944 * cache[i-6];
        neg += 39920 * cache[i-7];
        neg += 38440 * cache[i-9];
        neg += 1115136 * cache[i-10];
        neg += 3657600 * cache[i-13];
        neg += 331776 * cache[i-14];
        neg += 1658880 * cache[i-16];

        cache[i] = pos - neg;            /* pos > neg, so subtraction is safe with unsigned arithmetic */
    }

    cache_max = n;
    return cache[n];
}
