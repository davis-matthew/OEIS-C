/*
 * A037745  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0,1.
 * https://oeis.org/A037745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=1..n} d_k * 5^(n-k) where d_k cycles 2,3,0,1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (max index) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Pattern of base‑5 digits: 2, 3, 0, 1 repeated. */
    static const seq_t pat[4] = {2, 3, 0, 1};

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure storage for all terms up to n. */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap < n)
            new_cap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)           /* allocation failed */
            return 0;               /* undefined, but returned for safety */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len+1 up to n inclusively. */
    seq_t cur = (cache_len > 0) ? cache[cache_len - 1] : (seq_t)0;
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cur = cur * 5 + pat[(unsigned)((i - 1) % 4)];
        cache[i - 1] = cur;
    }
    cache_len = n;

    return cur;
}
