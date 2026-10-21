/*
 * A229584  Number of defective 3-colorings of an n X 6 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 50*a(n-1) - 805*a(n-2) + 4662*a(n-3) * - 12150*a(n-4) +
 *            14580*a(n-5) - 6561*a(n-6) * Initial values (for n = 1 … 6): *
 *            a(1)=40, a(2)=1620, a(3)=49848, * a(4)=1375152, a(5)=35690460,
 *            a(6)=890824020 * * The function uses memoisation in a static
 *            array so that calls may * occur in any order. I...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache     = NULL;   /* array of values 0 … n */
    static size_t cache_sz  = 0;      /* allocated slots (≥0)     */
    static idx_t  max_calc  = 0;      /* largest n already computed */

    /* First time: allocate space for at least indices 0…6. */
    if (cache == NULL) {
        cache_sz = (size_t)( (n > 6 ? n : 6) + 1 );   /* up to max(n,6)+1 */
        cache = (seq_t *)realloc(cache, cache_sz * sizeof(seq_t));
        if (!cache) return 0;                        /* allocation failure */

        /* Initialise the newly allocated part to 0. */
        for (size_t i = 0; i < cache_sz; ++i) cache[i] = 0;

        /* Base values. */
        cache[1] = 40;          /* a(1) */
        cache[2] = 1620;        /* a(2) */
        cache[3] = 49848;       /* a(3) */
        cache[4] = 1375152;     /* a(4) */
        cache[5] = 35690460;    /* a(5) */
        cache[6] = 890824020;   /* a(6) */
        max_calc = 6;
    }
    /* If n is beyond current allocation, grow the cache. */
    else if ((size_t)(n + 1) > cache_sz) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;                        /* allocation failure */

        /* Zero the newly added slots. */
        for (size_t i = cache_sz; i < new_sz; ++i) tmp[i] = 0;

        cache   = tmp;
        cache_sz = new_sz;
    }

    /* If the requested value is already computed, just return it. */
    if (n <= max_calc) return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        cache[i] = ((seq_t)50   ) * cache[i - 1]
                 - ((seq_t)805   ) * cache[i - 2]
                 + ((seq_t)4662  ) * cache[i - 3]
                 - ((seq_t)12150 ) * cache[i - 4]
                 + ((seq_t)14580 ) * cache[i - 5]
                 - ((seq_t)6561  ) * cache[i - 6];
    }
    max_calc = n;
    return cache[n];
}
