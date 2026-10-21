/*
 * A217450  Number of n X 1 arrays of the minimum value of corresponding
 *          elements and their horizontal and vertical neighbors in a random
 *          0..2 n X 1 array.
 * https://oeis.org/A217450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=3, a(3)=9, a(4)=17, a(n)=2*a(n-1)+a(n-4) for n>4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* External types provided by the build system */
extern seq_t a(idx_t n);

/* static cache of computed values */
static seq_t *cache = NULL;      /* array of a(1) .. a(cache_len) */
static size_t cache_len = 0;     /* current allocated length */
static idx_t  last_computed = 0; /* largest n already computed */

/* compute or retrieve a(n) */
seq_t a(idx_t n)
{
    if (n < 1)               /* undefined for n<1, return 0 as safe default */
        return (seq_t)0;

    /* make sure cache is large enough for indices up to n */
    if (n > cache_len) {
        size_t new_len = (size_t)n;
        /* realloc may be used; cast result for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)            /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        /* initialise newly allocated slots to 0 */
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* fill missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = (seq_t)3;
            else if (i == 2)
                val = (seq_t)3;
            else if (i == 3)
                val = (seq_t)9;
            else if (i == 4)
                val = (seq_t)17;
            else
                val = (seq_t)2 * cache[i - 2] + cache[i - 5];
            cache[i - 1] = val;
        }
        last_computed = n;
    }

    return cache[n - 1];
}
