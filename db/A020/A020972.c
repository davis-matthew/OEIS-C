/*
 * A020972  Expansion of 1/((1-7*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A020972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 28*a(n-1) - 255*a(n-2) + 756*a(n-3), n>=3;
 * initial values: a(0)=1, a(1)=28, a(2)=529.
 */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised terms */
    static size_t cache_size = 0;    /* number of terms stored */
    static size_t cache_cap  = 0;    /* capacity of allocated slice */

    size_t idx = (size_t)n;          /* index to compute */

    /* grow cache if necessary */
    if (idx >= cache_size) {
        size_t needed = idx + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? 2 * cache_cap : 8;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();       /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t start = cache_size;   /* first index to initialise */

        for (size_t i = start; i <= idx; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)28;
            } else if (i == 2) {
                cache[i] = (seq_t)529;
            } else {
                /* a(i) = 28*a(i-1) - 255*a(i-2) + 756*a(i-3) */
                seq_t term1 = (seq_t)756 * cache[i-3];
                seq_t term2 = (seq_t)28  * cache[i-1];
                seq_t term3 = (seq_t)255 * cache[i-2];
                cache[i] = term1 + term2 - term3;
            }
        }
        cache_size = idx + 1;
    }
    return cache[idx];
}
