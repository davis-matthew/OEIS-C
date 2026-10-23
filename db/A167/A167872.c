/*
 * A167872  A sequence of moments connected with Feynman numbers (A000698):
 *          Half the number of Feynman diagrams of order 2(n+1), for the
 *          electron self-energy in quantum electrodynamics (QED), i.e., all
 *          proper diagrams including Furry vanishing diagrams (those that
 *          vanish in 4-dimensional QED because of Furry...
 * https://oeis.org/A167872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (2*n-1)*a(n-1) + 2*Σ_{k=1..n} a(k-1)*a(n-k)
 *            (n>0) Recurrence from OEIS A167872
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t cache_len = 0;   /* number of allocated positions */

    /* If value already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Allocate / extend cache to hold n+1 values */
    idx_t new_len = n + 1;
    if (cache == NULL) {
#if defined(__cplusplus)
        cache = static_cast<seq_t*>(::malloc(new_len * sizeof(seq_t)));
#else
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
#endif
    } else {
#if defined(__cplusplus)
        cache = static_cast<seq_t*>(::realloc(cache, new_len * sizeof(seq_t)));
#else
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
    }
    if (!cache) {
        /* out of memory – caller should handle */
        return (seq_t)0;
    }

    /* Compute new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {           /* initial value */
            cache[0] = (seq_t)1;
            continue;
        }
        /* sum_{k=1..i} a(k-1)*a(i-k) = Σ_{j=0..i-1} a(j)*a(i-1-j) */
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < i; ++j)
            sum += cache[j] * cache[i - 1 - j];

        cache[i] = ((seq_t)(2 * i - 1)) * cache[i - 1] + ((seq_t)2) * sum;
    }

    cache_len = new_len;
    return cache[n];
}
