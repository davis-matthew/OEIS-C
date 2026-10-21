/*
 * A090978  a(n) = n + Sum{a(k): 1<=k<n and a(k) divides n}.
 * https://oeis.org/A090978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A090978(a) = a + Σ_{k<a, a(k) divides a} a(k)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache of computed terms, 1‑indexed */
static seq_t *cache = NULL;
static idx_t cache_len = 0;       /* highest index computed */
static size_t cache_cap = 0;      /* capacity of the cache */

/* grow the cache to accommodate at least n (1‑indexed) */
static void grow_cache(idx_t n) {
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_arr) {
        /* Allocation failed – abort (system will terminate program) */
        abort();
    }
    if (cache) {
        memcpy(new_arr, cache, (cache_len + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* compute a(n) for any n≥1, with memoisation */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for n<1, but guard */
    if (n <= cache_len) return cache[n];

    grow_cache(n);

    /* compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = (seq_t)i;                 /* start with n itself */
        seq_t i_seq = (seq_t)i;
        /* iterate over all previous k to test divisibility */
        for (idx_t k = 1; k < i; ++k) {
            seq_t ak = cache[k];
            /* ak divides i ? */
            if (ak != 0 && i_seq % ak == 0) {
                sum += ak;
            }
        }
        cache[i] = sum;
    }
    cache_len = n;
    return cache[n];
}
