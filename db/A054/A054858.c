/*
 * A054858  Number of basic blocks of size 5xn for tilings with square tiles
 *          of size up to 5 X 5.
 * https://oeis.org/A054858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3) for n>8, a(1)=1, a(2)=7, a(3)=13,
 *            a(4)=20, a(5)=35, a(6)=66, a(7)=118, a(8)=218
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the undefined case of n = 0 just in case */
    if (n == 0) return (seq_t)0;

    /* Cache of previously computed terms */
    static seq_t *cache = NULL;       /* pointer to the array */
    static idx_t cache_size = 0;      /* number of allocated slots   */
    static idx_t computed   = 0;      /* largest n that has been computed */

    /* Grow the cache if necessary */
    if (n > cache_size)
    {
        seq_t *new_cache;
#ifdef __cplusplus
        /* C++: use new[] which calls the constructors of seq_t */
        new_cache = new seq_t[n];
#else
        /* C: use malloc which allocates raw memory */
        new_cache = (seq_t *)malloc(sizeof(seq_t) * n);
#endif
        if (!new_cache) return (seq_t)0;   /* allocation failed */

        /* Copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Free the old cache */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_size = n;
    }

    /* Compute any terms that have not yet been computed */
    if (n > computed)
    {
        static const seq_t init[8] = {1, 7, 13, 20, 35, 66, 118, 218};

        for (idx_t i = computed + 1; i <= n; ++i)
        {
            if (i <= 8)
                cache[i - 1] = init[i - 1];
            else
                cache[i - 1] = cache[i - 2] + cache[i - 3] + cache[i - 4];
        }
        computed = n;
    }

    return cache[n - 1];
}
