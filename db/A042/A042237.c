/*
 * A042237  Denominators of continued fraction convergents to sqrt(644).
 * https://oeis.org/A042237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2,
   a(n)=coeff[n‑1]*a(n‑1)+a(n‑2)  for n≥2
   coeff = [2,1,1,1,6,1,1,1,2,50] (period 10)
   Denominators of continued‑fraction convergents to √644. */
#include <stdlib.h>
#include <stddef.h>

static const int coeff[] = {2,1,1,1,6,1,1,1,2,50};
static const int coeff_period = sizeof coeff / sizeof coeff[0];

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initial allocation */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                        /* a(0) */
        cache_len = 1;
    }

    /* extend cache up to requested index */
    size_t target = (size_t)n;
    while (cache_len <= target) {
        int ck = coeff[(cache_len - 1) % coeff_period];
        seq_t prev1 = cache[cache_len - 1];
        seq_t prev2 = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)0;

        seq_t cur = (seq_t)ck * prev1 + prev2;

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;  /* allocation failure */
            cache = tmp;
        }
        cache[cache_len] = cur;
        ++cache_len;
    }

    return cache[target];
}
