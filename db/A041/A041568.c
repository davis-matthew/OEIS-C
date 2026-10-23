/*
 * A041568  Numerators of continued fraction convergents to sqrt(302).
 * https://oeis.org/A041568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=17; for n≥1,
   a(n)=c[(n-1)%16]*a(n-1)+a(n-2),
   where c={2,1,1,1,4,2,1,16,1,2,4,1,1,1,2,34}.  */

#include <stdlib.h>

/* Periodic coefficients for the continued fraction of sqrt(302). */
static const unsigned int period[16] = {
    2, 1, 1, 1, 4, 2, 1, 16,
    1, 2, 4, 1, 1, 1, 2, 34
};

/* Cache of previously computed numerators. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

seq_t a(idx_t n)
{
    /* Return cached result if already computed. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Allocate or grow the cache to handle up to index n. */
    if (cache == NULL) {
        cache_cap = (n + 1 > 32 ? n + 1 : 32);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Prepare the two predecessor values for the recurrence. */
    idx_t start = cache_size;
    seq_t prev2, prev1;

    if (start == 0) {
        /* Base case: a(-2)=0, a(-1)=1. */
        prev2 = (seq_t)0;
        prev1 = (seq_t)1;
    } else if (start == 1) {
        /* a(-1)=1, a(0)=cache[0]. */
        prev2 = (seq_t)1;
        prev1 = cache[0];
    } else {
        prev2 = cache[start - 2];
        prev1 = cache[start - 1];
    }

    /* Compute terms from index 'start' up to n. */
    for (idx_t i = start; i <= n; ++i) {
        seq_t coeff = (i == 0) ? (seq_t)17
                               : (seq_t)period[(i - 1) % 16];
        seq_t curr = coeff * prev1 + prev2;
        cache[i] = curr;
        prev2 = prev1;
        prev1 = curr;
    }

    cache_size = n + 1;
    return cache[n];
}
