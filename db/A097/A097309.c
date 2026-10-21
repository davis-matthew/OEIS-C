/*
 * A097309  Chebyshev polynomials of the second kind, U(n,x), evaluated at
 *          x=13.
 * https://oeis.org/A097309
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A097309 – Chebyshev polynomials of the 2nd kind U(n,13)
    Recurrence:  a(-1)=0 , a(0)=1 ,
                  a(n)=26*a(n-1)-a(n-2)   (n≥1)
*/

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – not requested but safe  */
    if (n < -1)
        return (seq_t)0;

    /* For n=-1 return the specified value directly */
    if (n == -1)
        return (seq_t)0;

    /* Static cache to keep previously computed terms */
    static seq_t *cache      = NULL;   /* cache[i] holds a(i-2) for i≥2 */
    static size_t cache_size = 0;      /* allocated length of cache   */
    static idx_t max_computed = -1;    /* largest n that is known     */

    /* Ensure the cache is large enough to contain a(n) */
    size_t needed = (size_t)(n + 2);   /* indices 0 … n+1 inclusive */

    if (needed > cache_size) {
        /* Allocate new cache with the required size.
           For C++ builds use placement new to initialise objects. */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[needed];
#else
        seq_t *new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* unlikely in tests – abort early   */
#endif

        /* Copy any existing contents to the new cache and free the old one */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];   /* assignment works for both plain
                                            * and class types */
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }

        cache      = new_cache;
        cache_size = needed;

        /* Initialise the first two terms if this is the first allocation */
        if (max_computed < 0) {
            cache[0] = (seq_t)0;   /* a(-1)     */
            cache[1] = (seq_t)1;   /* a(0)      */
            max_computed = 0;
        }
    }

    /* Compute any missing terms up to the requested n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* a(k) = 26*a(k-1) - a(k-2) */
        cache[(size_t)(k + 1)] =
            (seq_t)26 * cache[(size_t)k] - cache[(size_t)(k - 1)];
    }

    max_computed = n;
    return cache[(size_t)(n + 1)];
}
