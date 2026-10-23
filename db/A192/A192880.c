/*
 * A192880  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          p(n,x) given in Comments.
 * https://oeis.org/A192880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 2*a(n-3) - a(n-4) (n>=4), a(0)=1,
 *            a(1)=0, a(2)=3, a(3)=7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t  size  = 0;  /* number of values cached */
    static size_t cap   = 0;  /* capacity of cache */

    /* initialise with base terms */
    if (cache == NULL) {
        cap  = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 3;
        cache[3] = 7;
        size = 4;
    }

    /* return if already computed */
    if (n < size) return cache[(size_t)n];

    /* grow cache if necessary */
    if ((size_t)cap <= (size_t)n) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = size; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] * (seq_t)2
                  + cache[i-2] * (seq_t)7
                  + cache[i-3] * (seq_t)2
                  - cache[i-4];
        cache[i] = val;
    }
    size = (idx_t)(n + 1);
    return cache[(size_t)n];
}
