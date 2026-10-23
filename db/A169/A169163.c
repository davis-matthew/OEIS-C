/*
 * A169163  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1: a(n) = 4 * Σ_{k=1}^{min(26,n)} a(n-k) – 10
 *            * a(n-27) (if n ≥ 27) + c(n), where c(n) = 2 for 1 ≤ n ≤ 26, 1
 *            for n = 27, 0 for n > 27. The function stores previously
 *            computed values in a static cache, so calls may arrive in any
 *            order and still be O(n) overall
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/alloc_seq_array.h"  /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"   /* void free_seq_array(seq_t *) */

static seq_t *cache   = NULL;   /* holds a(0),a(1),… */
static idx_t  cache_sz = 0;     /* number of terms stored */

/* enlarge cache to contain at least indices 0…n */
static void ensure_cache(idx_t n)
{
    if (n < cache_sz) return;          /* already present */

    idx_t new_sz = n + 1;
    seq_t *new_cache = alloc_seq_array((size_t)new_sz);
    if (!new_cache) return;            /* allocation failed – caller will abort */

    /* initialise new elements to 0 */
    for (idx_t i = 0; i < new_sz; ++i) new_cache[i] = 0;

    /* copy old values */
    if (cache) {
        for (idx_t i = 0; i < cache_sz; ++i) new_cache[i] = cache[i];
        free_seq_array(cache);
    }

    new_cache[0] = 1;                  /* a(0) = 1 */
    cache = new_cache;
    cache_sz = new_sz;
}

seq_t a(idx_t n)
{
    const seq_t ONE  = 1;
    const seq_t TWO  = 2;
    const seq_t FOUR = 4;
    const seq_t TEN  = 10;

    ensure_cache(n);

    /* compute terms that are not yet in the cache */
    for (idx_t i = 1; i <= n; ++i) {
        if (i < cache_sz && cache[i] != (seq_t)0) continue; /* already computed */

        seq_t sum = 0;
        idx_t kmax = (i < 26 ? i : 26);        /* use only existing previous terms */

        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k];

        seq_t val = sum * FOUR;

        if (i >= 27)
            val -= cache[i - 27] * TEN;

        if (i <= 26)
            val += TWO;          /* i = 1 … 26 */
        else if (i == 27)
            val += ONE;          /* i = 27 */

        cache[i] = val;
    }

    return cache[n];
}
