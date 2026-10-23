/*
 * A169168  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1.
   For 1≤n≤26   : a(n)=10*a(n‑1)  (with a(1)=11).
   a(27)        : a(27)=9*∑_{i=1}^{26}a(i) – 45*a(0) + 1.
   For n≥28     : a(n)=9*∑_{i=1}^{26}a(n‑i) – 45*a(n‑27).   */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;     /* memoised values                */
static size_t cache_size = 0;   /* allocated length of cache array*/
static size_t computed = 0;     /* first index yet to be computed */

/* external linkage as required */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;            /* target index               */

    /* enlarge cache if necessary */
    if (target >= cache_size) {
        size_t newlen = cache_size ? cache_size * 2 : 32;
        if (newlen <= target) newlen = target + 1;
        seq_t *tmp = seq_realloc(cache, cache_size, newlen);
        if (!tmp) exit(1);                /* abort on allocation failure */
        cache     = tmp;
        cache_size = newlen;
    }

    /* initialise first value on first use */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;                     /* next index to compute      */
    }

    /* compute missing values up to target */
    for (size_t i = computed; i <= target; ++i) {
        if (i == 1) {
            cache[1] = cache[0] * (seq_t)10 + (seq_t)1;
        } else if (i <= 26) {
            cache[i] = cache[i-1] * (seq_t)10;
        } else if (i == 27) {
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= 26; ++j)
                sum += cache[j];
            cache[27] = sum * (seq_t)9 - (seq_t)45 * cache[0] + (seq_t)1;
        } else { /* i ≥ 28 */
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= 26; ++j)
                sum += cache[i-j];
            cache[i] = sum * (seq_t)9 - (seq_t)45 * cache[i-27];
        }
    }
    computed = target + 1;

    return cache[target];
}
