/*
 * A178827  Partial sums of floor(8^n/9)/7.
 * https://oeis.org/A178827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 9*a(n-1) - 7*a(n-2) - 9*a(n-3) + 8*a(n-4) (n > 4) a(1) =
 *            0, a(2) = 1, a(3) = 9, a(4) = 74
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for malloc, realloc */

/* static cache of computed values */
static seq_t *cache = NULL;      /* pointer to array a(1)...a(max)  */
static idx_t cache_len = 0;      /* number of terms stored (a(1) .. a(cache_len)) */

/* ensure the cache can hold at least n terms */
static void ensure_cache(idx_t n) {
    if (cache_len >= n) return;
    idx_t new_cap = n > 4 ? n : 4;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure – unlikely in tests */
    cache = new_cache;
    /* if the cache was empty, initialise base values */
    if (cache_len == 0) {
        cache[0] = 0;                 /* a(1)   */
        if (new_cap >= 2) cache[1] = 1;/* a(2)   */
        if (new_cap >= 3) cache[2] = 9;/* a(3)   */
        if (new_cap >= 4) cache[3] = 74;/* a(4)   */
        cache_len = (new_cap >= 4) ? 4 : (idx_t)new_cap;
    }
    /* If the cache already had some values, the base values are unchanged.
       Now compute any new terms required. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 4) continue;        /* base values already set */
        /* recurrence: a(i) = 9*a(i-1) - 7*a(i-2) - 9*a(i-3) + 8*a(i-4) */
        seq_t t1 = cache[i-2] * (seq_t)9;
        seq_t t2 = cache[i-3] * (seq_t)7;
        seq_t t3 = cache[i-4] * (seq_t)9;
        seq_t t4 = cache[i-5] * (seq_t)8;
        cache[i-1] = t1 - t2 - t3 + t4;
    }
    cache_len = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used for n>=1 but harmless */
    ensure_cache(n);
    return cache[n-1];              /* a(1) is stored at index 0 */
}
