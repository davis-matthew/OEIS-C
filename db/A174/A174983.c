/*
 * A174983  y-values in the solution to x^2-53*y^2=1.
 * https://oeis.org/A174983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 132498*a(n-1) - a(n-2), a(1) = 0, a(2) = 9100
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Protect against out‑of‑range indices  */
    if (n < 1) return (seq_t)0;

    /*  Convert the index to a platform type for array access   */
    size_t idx = (size_t)n;

    /*  Static cache of computed terms.
        cache[i] stores a(i)  (1‑based indexing)          */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;   /* allocated slots (>= idx+2)   */
    static idx_t  used   = 0;   /* highest index already computed   */

    /*  Ensure the cache is large enough                 */
    if (cap < idx + 2) {
        size_t new_cap = idx + 2;
        if (cap > 0 && new_cap < cap * 2) new_cap = cap * 2;

        /*  realloc requires a cast when building under C++   */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure: return 0 */

        cache = tmp;
        cap   = new_cap;
    }

    /*  Initialise the first two known terms if needed */
    if (used < 2) {
        cache[1] = (seq_t)0;
        if (cap >= 3) cache[2] = (seq_t)9100;
        used = 2;
    }

    /*  Compute missing terms up to the requested index   */
    for (size_t i = (size_t)used + 1; i <= idx; ++i) {
        cache[i] = cache[i - 1] * (seq_t)132498 - cache[i - 2];
    }
    used = (idx_t)idx;

    return cache[idx];
}
