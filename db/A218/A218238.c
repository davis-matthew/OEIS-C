/*
 * A218238  Hilltop maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any king-move
 *          neighbor in a random 0..3 n X 4 array.
 * https://oeis.org/A218238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7));
 *            a(1)=15, a(2)=255, a(3)=4095, a(4)=65535, a(5)=1048545,
 *            a(6)=16776705, a(7)=268427265
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = {
    15ULL,          /* a(1) */
    255ULL,         /* a(2) */
    4095ULL,        /* a(3) */
    65535ULL,       /* a(4) */
    1048545ULL,     /* a(5) */
    16776705ULL,    /* a(6) */
    268427265ULL    /* a(7) */
};

static seq_t *cache = NULL;      /* 1‑based cache of computed values */
static idx_t cache_size = 0;     /* highest n whose value is in cache */
static idx_t cache_cap  = 0;     /* allocated capacity (1‑based) */

static void init_cache(void)
{
    if (cache) return;
    idx_t cap = 8;                              /* allow indices up to 7 initially */
    cache = (seq_t*)malloc(sizeof(seq_t) * (cap + 1));   /* +1 for 1‑based indexing */
    cache_cap = cap;
    for (idx_t i = 0; i < 7; ++i) cache[i + 1] = init[i];
    cache_size = 7;
}

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;               /* nothing to do */

    if (cache_cap < n) {                       /* need more space */
        idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < n) new_cap = n;
        seq_t *p = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        cache = p;
        cache_cap = new_cap;
    }

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int i = 1; i <= 7; ++i)
            sum += ((seq_t)15) * cache[k - i];
        cache[k] = sum;
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n < 1 */
    init_cache();
    ensure_cache(n);
    return cache[n];
}
