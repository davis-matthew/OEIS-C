/*
 * A034910  One quarter of octo-factorial numbers.
 * https://oeis.org/A034910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(8*n-4), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE_N 10001   /* up to about 10 000 as required */

/* Compute a(n) for the OEIS sequence A034910 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */

    /* ----- Cached computation for n ≤ MAX_CACHE_N ----- */
    if (n <= MAX_CACHE_N) {
        /* Static cache with 1‑based indexing */
        static seq_t cache[MAX_CACHE_N + 1];
        static idx_t cache_len = 0;     /* highest n already computed */

        if (cache_len == 0) {           /* initialise a(1) */
            cache[1] = (seq_t)1;
            cache_len = 1;
        }

        /* extend the cache up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i)
            cache[i] = cache[i - 1] * ((seq_t)(8 * i - 4));

        cache_len = n;
        return cache[n];
    }

    /* ----- Direct computation for larger n ----- */
    seq_t result = (seq_t)1;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        result = result * ((seq_t)(8 * i - 4));

    return result;
}
