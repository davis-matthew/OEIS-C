/*
 * A077839  Expansion of (1 - x)/(1 - 3*x - 2*x^2 - 2*x^3).
 * https://oeis.org/A077839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + 2*a(n-3) for n >= 3 with a(0) = 1,
 *            a(1) = 2, a(2) = 8
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding the computed terms */
    static seq_t *cache = NULL;          /* array of terms */
    static idx_t alloc_len = 0;          /* allocated size of cache   */
    static idx_t comp_len = 0;           /* number of terms already computed (highest index + 1) */

    /* enlarge allocation if necessary */
    if (n + 1 > alloc_len) {
        idx_t new_len = alloc_len ? alloc_len * 2 : 1;
        if (new_len <= alloc_len)       /* avoid overflow when alloc_len is 0 */
            new_len = n + 1;
        if (new_len < n + 1)
            new_len = n + 1;
        /* Allocate or reallocate with proper cast for C++ */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache)   /* out of memory – abort */
            exit(1);
        cache = new_cache;
        alloc_len = new_len;
    }

    /* compute missing terms up to n */
    if (n + 1 > comp_len) {
        for (idx_t i = comp_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)2;
            else if (i == 2)
                cache[i] = (seq_t)8;
            else {
                seq_t t1 = cache[i - 1];
                seq_t t2 = cache[i - 2];
                seq_t t3 = cache[i - 3];
                cache[i] = 3 * t1 + 2 * t2 + 2 * t3;
            }
        }
        comp_len = n + 1;
    }

    return cache[n];
}
