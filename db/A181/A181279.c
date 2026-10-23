/*
 * A181279  Number of 3 X n binary matrices M with rows in strictly increasing
 *          order and rows of M*Mtranspose (mod 2) in strictly decreasing
 *          order.
 * https://oeis.org/A181279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 9, a(n) = 14*a(n-1) - 36*a(n-2) - 216*a(n-3) +
 *            1056*a(n-4) - 384*a(n-5) - 3584*a(n-6) + 4096*a(n-7). Initial
 *            values: a(1) = 0, a(2) = 0, a(3) = 9, a(4) = 79, a(5) = 766,
 *            a(6) = 6060, a(7) = 49928, a(8) = 395024, a(9) = 3170784
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values, indexed by n (1‑based) */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static size_t cache_sz = 0;     /* current allocated size (n+1) */
    static idx_t   max_computed = 0;/* highest n whose value is known */

    /* initialize cache with first 9 terms */
    if (cache_sz == 0) {
        cache_sz = 10; /* indices 0..9 */
        cache = (seq_t *)calloc(cache_sz, sizeof(seq_t));
        /* pre‑computed terms */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)79;
        cache[5] = (seq_t)766;
        cache[6] = (seq_t)6060;
        cache[7] = (seq_t)49928;
        cache[8] = (seq_t)395024;
        cache[9] = (seq_t)3170784;
        max_computed = 9;
    }

    if (n <= max_computed) {
        return cache[n];
    }

    /* enlarge cache if needed */
    if ((size_t)(n + 1) > cache_sz) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure: return 0 (unlikely in testing) */
            return (seq_t)0;
        }
        cache = tmp;
        cache_sz = new_sz;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = 14 * cache[i-1];
        seq_t t2 = 36 * cache[i-2];
        seq_t t3 = 216 * cache[i-3];
        seq_t t4 = 1056 * cache[i-4];
        seq_t t5 = 384 * cache[i-5];
        seq_t t6 = 3584 * cache[i-6];
        seq_t t7 = 4096 * cache[i-7];
        cache[i] = t1 - t2 - t3 + t4 - t5 - t6 + t7;
    }
    max_computed = n;
    return cache[n];
}
