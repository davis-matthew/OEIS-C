/*
 * A239485  Number of (5,0)-separable partitions of n; see Comments.
 * https://oeis.org/A239485
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{f=1}^{⌊n/5⌋} ( p_{f+1}(n-5f) - p_{f}(n-5f-5) ) where
 *            p_k(m) = number of partitions of m into exactly k positive
 *            integers and p_k satisfies p_k(m) = p_k(m-k) + p_{k-1}(m-1)
 *            (with p_0(0)=1, p_k(0)=0 for k>0) The subtraction removes
 *            partitions that contain a part equal to 5
 * Range:     u8 n <= 40, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values (for n>=6) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 5;           /* highest n whose value is stored */

    if (n <= max_computed) {
        /* value already computed */
        return cache[n];
    }

    /* enlarge cache if necessary */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failed */
        cache  = new_cache;
        cache_cap = new_cap;
    }
    /* initialise new entries to 0 */
    for (size_t i = (size_t)max_computed + 1; i <= (size_t)n; ++i)
        cache[i] = 0;
    size_t old_max = (size_t)max_computed;
    max_computed = n;

    /* Arrays for partition counts p_k(m) */
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!prev || !cur) return 0;            /* allocation failed */

    /* initialise p_1(m) : 1 way for m>0, 0 for m=0 */
    prev[0] = 0;
    for (size_t s = 1; s <= (size_t)n; ++s)
        prev[s] = 1;

    /* k corresponds to f+1, start at k=2 (f=1) */
    size_t k_end = n / 5 + 1;
    for (size_t k = 2; k <= k_end; ++k) {
        /* maximum s such that n-5f = s with f = k-1 */
        size_t maxS = (size_t)n - 5 * (k - 1);

        /* compute p_k(s) for s = 0..maxS */
        cur[0] = 0;
        for (size_t s = 1; s <= maxS; ++s) {
            seq_t term1 = (s >= k) ? cur[s - k] : 0;       /* p_k(s-k) */
            seq_t term2 = prev[s - 1];                     /* p_{k-1}(s-1) */
            cur[s] = term1 + term2;
        }

        /* update a(n) for new indices */
        for (size_t s = 0; s <= maxS; ++s) {
            seq_t g = cur[s];
            if (s >= 5) g -= prev[s - 5];                  /* g_k(s) = p_k(s)-p_{k-1}(s-5) */

            size_t val = 5 * (k - 1) + s;                  /* corresponding n */
            if (val > old_max && val <= (size_t)n) {
                cache[val] += g;
            }
        }

        /* prepare for next k */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    free(prev);
    free(cur);

    return cache[n];
}
