/*
 * A041762  Numerators of continued fraction convergents to sqrt(402).
 * https://oeis.org/A041762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 802*a(n-2) - a(n-4) a(0) = 20, a(1) = 401, a(2) = 16060,
 *            a(3) = 321601
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order and build the
       sequence on demand. */
    static seq_t *cache = NULL;   /* array of computed terms           */
    static idx_t cache_len = 0;   /* number of terms currently in cache*/
    static idx_t cache_cap = 0;   /* allocated capacity                */

    /* First call: allocate cache and fill the initial four terms. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely */

        /* initial terms a(0) … a(3) */
        cache[0] = 20;
        cache[1] = 401;
        cache[2] = 16060;
        cache[3] = 321601;
        cache_len = 4;
    }

    /* Return if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Ensure there is enough space for n+1 terms. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prod = 802 * cache[i - 2];   /* 802 • a(i–2) */
        prod -= cache[i - 4];              /* – a(i–4)     */
        cache[i] = prod;
    }

    cache_len = n + 1;
    return cache[n];
}
