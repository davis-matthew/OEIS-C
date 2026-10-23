/*
 * A042363  Denominators of continued fraction convergents to sqrt(708).
 * https://oeis.org/A042363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1; a(n)=coeff[n‑1]*a(n‑1)+a(n‑2)
   where coeff repeats [1,1,1,1,4,4,4,1,1,1,1,52] for n≥1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* periodic continued‑fraction coefficients for sqrt(708) */
    static const int coeff[12] = {1,1,1,1,4,4,4,1,1,1,1,52};
    const size_t period = 12;

    /* initialise cache */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failed */
        cache[0] = (seq_t)1;               /* q_0 = 1 */
        cache_len = 1;
    }

    size_t target = (size_t)n;
    if (target >= cache_len) {
        /* grow capacity if necessary */
        if (target >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= target) new_cap <<= 1;   /* double until enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;                 /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute new terms */
        for (size_t k = cache_len; k <= target; ++k) {
            seq_t prev1 = cache[k - 1];
            seq_t prev2 = (k >= 2) ? cache[k - 2] : (seq_t)0;
            int ak = coeff[(k - 1) % period];
            seq_t next = (seq_t)ak * prev1 + prev2;
            cache[k] = next;
        }
        cache_len = target + 1;
    }

    return cache[target];
}
