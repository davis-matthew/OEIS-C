/*
 * A079957  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={0,1,3}.
 * https://oeis.org/A079957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5) + a(n-6) with a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 28, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache for indices 0..6 */
    if (cache_len == 0) {
        cache_len = 7;                     /* 0..6 inclusive */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) {
            return (seq_t)0;               /* allocation failure – unlikely in test harness */
        }
        for (idx_t i = 0; i < cache_len; ++i) {
            cache[i] = (seq_t)0;
        }
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        /* a(1), a(2) remain 0 */

        /* Compute values up to 6 inclusive using recurrence */
        for (idx_t i = 3; i < cache_len; ++i) {
            seq_t val = (seq_t)0;
            if (i >= (idx_t)3) val += cache[i - 3];
            if (i >= (idx_t)5) val += cache[i - 5];
            if (i >= (idx_t)6) val += cache[i - 6];
            cache[i] = val;
        }
    }

    /* Grow cache if needed to cover index n */
    if (n + (idx_t)1 > cache_len) {
        idx_t new_len = n + (idx_t)1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            return (seq_t)0;               /* allocation failure – unlikely in test harness */
        }
        /* Initialize newly added entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            new_cache[i] = (seq_t)0;
        }
        /* Compute missing values using recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t val = (seq_t)0;
            if (i >= (idx_t)3) val += new_cache[i - 3];
            if (i >= (idx_t)5) val += new_cache[i - 5];
            if (i >= (idx_t)6) val += new_cache[i - 6];
            new_cache[i] = val;
        }
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
