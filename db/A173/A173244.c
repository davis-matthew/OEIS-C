/*
 * A173244  G.f. (x^10 +x^9 +x^8 +.... +x+1) / (x^10 +x^9 -x^7 -x^6 -x^5 -x^4
 *          -x^3 +x +1).
 * https://oeis.org/A173244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)-a(n-8)-a(n-9)+a(n-11) + (n==0?1 :
 *            (n==11?-1:0)) (recurrence from
 *            (1-x^11)/(1-x^2-x^3+x^8+x^9-x^11)) a(0) = 1 a(1) = 0, a(2) = 1,
 *            … This implementation memoises values in a static array so that
 *            subsequent calls on any index n >= 0 are O(1) after the initial
 *            computation. It wor...
 * Range:     u8 n <= 34, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* stores a(0) .. a(n) values */
static size_t cache_size = 0; /* number of elements currently allocated */

/* External linkage required by specification */
seq_t a(idx_t n)
{
    size_t n_sz = (size_t)n;

    /* Allocate or grow the cache to hold index n */
    if (cache_size <= n_sz) {
        size_t old_size = cache_size;
        size_t new_size = n_sz + 1;

        /* realloc returns void* in C, require cast for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache = tmp;
        cache_size = new_size;

        /* Compute all missing values from old_size up to n_sz */
        for (size_t i = old_size; i <= n_sz; ++i) {
            seq_t val = 0;

            if (i >= 2) val += cache[i - 2];
            if (i >= 3) val += cache[i - 3];
            if (i >= 8) val -= cache[i - 8];
            if (i >= 9) val -= cache[i - 9];
            if (i >= 11) val += cache[i - 11];

            /* Contributions from the numerator 1 - x^11 */
            if (i == 0)        val += 1;   /* +1 at x^0 */
            else if (i == 11)  val -= 1;   /* -1 at x^11 */

            cache[i] = val;
        }
    }

    return cache[n_sz];
}
