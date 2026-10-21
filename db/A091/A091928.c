/*
 * A091928  a(0)=1, a(1)=5; a(n) = 6*a(n-1) + 5*a(n-2) for n > 1.
 * https://oeis.org/A091928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=5; recurrence: a(n) = 6*a(n-1) + 5*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc/realloc */

static seq_t *a_cache = NULL;      /* cache for computed values */
static idx_t   a_cache_len = 0;    /* number of cached entries */

seq_t a(idx_t n)
{
    /* Return from cache if already computed */
    if (n < a_cache_len)
        return a_cache[n];

    /* Grow cache to hold indices 0..n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!tmp)             /* memory allocation failed */
        return (seq_t)0; /* silence, caller may not care */
    a_cache = tmp;

    /* Compute any missing entries */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[i] = (seq_t)1;
        } else if (i == 1) {
            a_cache[i] = (seq_t)5;
        } else {
            seq_t t1 = a_cache[i-1];
            t1 *= (seq_t)6;
            seq_t t2 = a_cache[i-2];
            t2 *= (seq_t)5;
            a_cache[i] = t1 + t2;
        }
    }

    a_cache_len = new_len;
    return a_cache[n];
}
