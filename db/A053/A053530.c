/*
 * A053530  Expansion of e.g.f.: exp(-x - x^2/2 + x*exp(x)).
 * https://oeis.org/A053530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0. For n>=2, a(n)=∑_{s=1}^{n-1} C(n-1,s) * w(s+1) *
 *            a(n-s-1), where w(2)=1 and w(k)=k for k>2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No additional headers besides those available in C99/C++11. */
#include <stdlib.h>

/* The function below computes the OEIS sequence A053530.  It uses a
   static cache so that calls with arbitrary n are answered in O(n)
   time overall.  It only uses integer arithmetic; no floats or
   library calls other than standard allocation functions.  */

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  We grow it as needed. */
    static seq_t *cache = NULL;            /* Pointer to array of seq_t. */
    static idx_t cache_len = 0;            /* Number of terms that are valid in cache. */
    static idx_t cache_cap = 0;            /* Current allocation size (in terms). */

    /* Ensure that cache is large enough to hold terms up to n. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise the first two terms if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;        /* a(0)=1 */
        cache[1] = (seq_t)0;        /* a(1)=0 */
        cache_len = 2;
    }

    /* Compute all terms missing up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        if (i == 1) { cache[1] = (seq_t)0; continue; }

        seq_t sum = 0;
        seq_t binom = 1;           /* Current binomial coefficient C(i-1,0). */

        /* Outer loop over s = number of leaves in the star chosen first. */
        for (idx_t s = 1; s <= i - 1; ++s) {
            /* Update binom to C(i-1,s) using multiplicative formula. */
            binom = (seq_t)((binom * (seq_t)(i - s)) / (seq_t)s);

            /* Weight w for a star of size (s+1).  Size 2 contributes 1,
               larger sizes contribute their own size. */
            seq_t w = ((s + 1) == (idx_t)2) ? (seq_t)1 : (seq_t)(s + 1);

            /* Add contribution of this choice. */
            sum += binom * w * cache[i - s - 1];
        }

        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
