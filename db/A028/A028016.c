/*
 * A028016  Expansion of 1/((1-2x)(1-8x)(1-9x)(1-11x)).
 * https://oeis.org/A028016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*11^(n+3)-3*9^(n+4)+21*8^(n+3)-2^(n+3))/378
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    /* Simple memoised recurrence:
       a(0)=1, a(1)=30, a(2)=585, a(3)=9410
       a(n)=30*a(n-1)-315*a(n-2)+1310*a(n-3)-1584*a(n-4)   for n≥4
    */
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t  cached = 0;        /* largest index computed so far */

    /* initialise cache with first four values */
    if (cache == NULL) {
        size_t sz = 4;                /* at least four entries for 0..3 */
        if (n + 1 > sz)
            sz = n + 1;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory: fall back to 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)585;
        cache[3] = (seq_t)9410;
        cached = 3;
    }

    /* If we already have the requested value, return it */
    if (n <= cached)
        return cache[n];

    /* Grow the cache to hold all values up to n */
    size_t needed = (size_t)(n + 1);
    size_t old_size = (size_t)(cached + 1);
    if (needed > old_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cached = 0;
            return (seq_t)0;           /* allocation failed */
        }
        cache = tmp;
    }

    /* Compute the new terms using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* Use 128‑bit arithmetic for intermediate to avoid overflow
           for the supported seq_t sizes (uint64_t, uint128_t).  For
           arbitrary‑precision seq_t this cast does nothing. */
        __int128 prod1 = (__int128)30    * (__int128)cache[i - 1];
        __int128 prod2 = (__int128)315   * (__int128)cache[i - 2];
        __int128 prod3 = (__int128)1310  * (__int128)cache[i - 3];
        __int128 prod4 = (__int128)1584  * (__int128)cache[i - 4];
        __int128 val   = prod1 - prod2 + prod3 - prod4;
        cache[i] = (seq_t)val;          /* truncate if necessary */
    }
    cached = n;
    return cache[n];
}
