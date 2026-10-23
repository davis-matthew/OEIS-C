/*
 * A010790  a(n) = n!*(n+1)!.
 * https://oeis.org/A010790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n! * (n+1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;    /* array of computed terms      */
static idx_t cap     = 0;       /* current capacity in terms    */
static idx_t maxComputed = 0;   /* largest index computed       */

/* Return the n-th term of OEIS A010790: n! * (n+1)! */
seq_t a(idx_t n)
{
    /* Resize the cache if necessary */
    if (n >= cap) {
        idx_t newCap = n + 1;
        /* Use realloc; cast to (seq_t*) for C++20 compatibility */
        seq_t *newCache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        cache = newCache;
        cap   = newCap;
    }

    /* If this is the first computation, initialise a(0) */
    if (maxComputed == 0 && n >= 0) {
        cache[0] = 1;           /* a(0) = 0! * 1! = 1 */
        maxComputed = 0;
    }

    /* Compute any missing values using the recurrence
       a(i) = a(i-1) * i * (i+1)   for i >= 1 */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val = mul_small(val, (unsigned int)i);        /* multiply by i   */
        val = mul_small(val, (unsigned int)(i + 1));  /* multiply by i+1 */
        cache[i] = val;
    }
    maxComputed = (n > maxComputed) ? n : maxComputed;

    return cache[n];
}
