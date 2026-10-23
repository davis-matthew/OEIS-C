/*
 * A181248  Number of nX5 binary matrices with no 2X2 block having four 1's.
 * https://oeis.org/A181248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            20*a(n-1)+188*a(n-2)-192*a(n-3)-1660*a(n-4)+2804*a(n-5)-507*a(n-6)-624*a(n-7)
 *            Initial values: a(1)=32, a(2)=819, a(3)=22077, a(4)=587920,
 *            a(5)=15701273, a(6)=419045269, a(7)=11185495872
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of previously computed values   */
static idx_t  cache_size = 0;        /* number of elements stored in cache   */
static idx_t  cache_cap  = 0;        /* current allocated capacity           */

/* First 7 terms of the sequence */
static const seq_t init_vals[7] = {
    (seq_t)32ULL,
    (seq_t)819ULL,
    (seq_t)22077ULL,
    (seq_t)587920ULL,
    (seq_t)15701273ULL,
    (seq_t)419045269ULL,
    (seq_t)11185495872ULL
};

/* Ensure cache has at least 'need' elements; grow as necessary */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real program we would handle allocation failure here. */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function and memoisation */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1, return 0 as placeholder */

    /* Initialise cache on first call */
    if (cache_size == 0) {
        ensure_cache_capacity(7);
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_size = 7;
    }

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* Compute missing terms up to n */
    for (idx_t cur = cache_size + 1; cur <= n; ++cur) {
        /* Use recurrence:
         * a(cur) = 20*a(cur-1) + 188*a(cur-2) - 192*a(cur-3)
         *          - 1660*a(cur-4) + 2804*a(cur-5) - 507*a(cur-6)
         *          - 624*a(cur-7)
         */
        const idx_t i1 = cur - 2; /* cur-1 */
        const idx_t i2 = cur - 3; /* cur-2 */
        const idx_t i3 = cur - 4; /* cur-3 */
        const idx_t i4 = cur - 5; /* cur-4 */
        const idx_t i5 = cur - 6; /* cur-5 */
        const idx_t i6 = cur - 7; /* cur-6 */
        const idx_t i7 = cur - 8; /* cur-7 */

        seq_t pos = (seq_t)20 * cache[i1] + (seq_t)188 * cache[i2] + (seq_t)2804 * cache[i5];
        seq_t term = pos;
        term -= (seq_t)192 * cache[i3];
        term -= (seq_t)1660 * cache[i4];
        term -= (seq_t)507 * cache[i6];
        term -= (seq_t)624 * cache[i7];

        ensure_cache_capacity(cur);
        cache[cur - 1] = term;
    }
    cache_size = n; /* all terms up to 'n' are now stored */

    return cache[n - 1];
}
