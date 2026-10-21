/*
 * A081123  a(n) = floor(n/2)!.
 * https://oeis.org/A081123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/2)!
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 25, u64 n <= 41, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* fact_cache[i] = i! for i already computed */
static idx_t   cache_size = 0;     /* number of factorials stored (fact_cache[0..cache_size-1]) */

/* Return floor(n/2)!  –  n is non‑negative, offset 0. */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t k = n >> 1;          /* same as n/2 for unsigned types */

    /* Grow the cache if necessary */
    if (k >= cache_size) {
        idx_t new_size = k + 1;            /* we need indices 0..k inclusive */

        /* Reallocate array; cast to seq_t* to satisfy C++ */
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                       /* allocation failed – abort */
        fact_cache = tmp;

        /* Initialise base case if cache was empty */
        if (cache_size == 0) {
            fact_cache[0] = (seq_t)1;      /* 0! = 1 */
        }

        /* Compute new factorials */
        idx_t start = (cache_size == 0) ? 1 : cache_size;   /* skip 0 if already set */
        for (idx_t i = start; i <= k; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }

        cache_size = new_size;
    }

    return fact_cache[k];
}
