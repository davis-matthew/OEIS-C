/*
 * A206515  Number of (n+1) X 3 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than four equal edges, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A206515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 66*a(n-1) -116*a(n-2) -1310*a(n-3) -7016*a(n-4)
 *            -9076*a(n-5) -9408*a(n-6) -4662*a(n-7) -4635*a(n-8) -378*a(n-9)
 *            -648*a(n-10) This recurrence is valid for n ≥ 11. The first 10
 *            terms are given explicitly
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* offset is 1 – lower indices are undefined */

    /* static cache – 1 based indexing, cache[0] unused */
    static seq_t *cache      = NULL;   /* array of computed values */
    static size_t cache_cap  = 0;      /* allocated capacity (number of slots)   */
    static size_t cache_size = 0;      /* highest index with a defined value */

    /* initial values for n = 1 … 10 */
    static const seq_t init_vals[11] = {
        (seq_t)0,                      /* placeholder for index 0 */
        (seq_t)186,
        (seq_t)10998,
        (seq_t)695541,
        (seq_t)44350086,
        (seq_t)2830665213,
        (seq_t)180689239504,
        (seq_t)11534052523389,
        (seq_t)736261766040550,
        (seq_t)46998354497911525,
        (seq_t)3000081595834713716
    };

    /* initialise cache on first call */
    if (cache_size == 0) {
        /* allocate a few slots – make index 0 unused */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – return 0 */

        /* copy the initial 10 terms */
        for (size_t i = 1; i <= 10; ++i)
            cache[i] = init_vals[i];

        cache_size = 10;   /* cache now holds values up to 10 */
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms only for n ≥ 11 */
    size_t start = cache_size + 1;
    if (start < 11) start = 11;   /* avoid applying recurrence to indices ≤10 */

    if (n > cache_size) {
        for (idx_t i = (idx_t)start; i <= n; ++i) {
            seq_t pos = ((seq_t)66)   * cache[i  - 1];
            seq_t neg = ((seq_t)116)  * cache[i  - 2];
            neg += ((seq_t)1310)     * cache[i  - 3];
            neg += ((seq_t)7016)     * cache[i  - 4];
            neg += ((seq_t)9076)     * cache[i  - 5];
            neg += ((seq_t)9408)     * cache[i  - 6];
            neg += ((seq_t)4662)     * cache[i  - 7];
            neg += ((seq_t)4635)     * cache[i  - 8];
            neg += ((seq_t)378)      * cache[i  - 9];
            neg += ((seq_t)648)      * cache[i -10];

            cache[i] = pos - neg;    /* pos > neg for all n ≥ 11 */
        }
        cache_size = (size_t)n;
    }

    return cache[n];
}
