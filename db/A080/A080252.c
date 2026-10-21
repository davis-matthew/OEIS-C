/*
 * A080252  a(n) = n*a(n-1)+4*a(n-2)-4*(n-2)*a(n-3).
 * https://oeis.org/A080252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*a(n-1) + 4*a(n-2) - 4*(n-2)*a(n-3),
 * a(0) = 0, a(1) = 1, a(2) = 2
 * (OEIS A080252)
 */
#include <stdlib.h>

/* external types provided by the build */
extern int _dummy; /* to avoid unused warning when seq_t/idx_t are macros */

seq_t a(idx_t n)
{
    /* Simple memoised recurrence implementation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* currently allocated size   */
    static idx_t cache_used = 0;   /* number of computed terms    */

    /* Ensure we have enough space. */
    if (cache_size <= n) {
        idx_t newsize = cache_size ? cache_size * 2 : 16;
        while (newsize <= n)
            newsize *= 2;
        cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        cache_size = newsize;
    }

    /* Initialise base values if not already set. */
    if (cache_used == 0) {
        cache[0] = (seq_t)0;
        cache_used = 1;
    }
    if (cache_used == 1 && n >= 1) {
        cache[1] = (seq_t)1;
        cache_used = 2;
    }
    if (cache_used == 2 && n >= 2) {
        cache[2] = (seq_t)2;
        cache_used = 3;
    }

    /* If the requested term is already computed, return it. */
    if (n < cache_used)
        return cache[n];

    /* Compute remaining terms up to n. */
    for (idx_t k = cache_used; k <= n; ++k) {
        seq_t term = ( (seq_t)k      * cache[k-1] )
                   + ( (seq_t)4      * cache[k-2] )
                   - ( (seq_t)4      * ((seq_t)(k-2)) * cache[k-3] );
        cache[k] = term;
    }

    cache_used = n + 1;
    return cache[n];
}
