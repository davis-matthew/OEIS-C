/*
 * A208035  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 6*a(n-3) - a(n-4) - a(n-6) for n >
 *            7 Initial terms: a(1)=9, a(2)=81, a(3)=289, a(4)=961, a(5)=3249,
 *            a(6)=11025, a(7)=37249
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static const seq_t base[8] = {
    (seq_t)0,        // unused for index 0
    (seq_t)9,        // a(1)
    (seq_t)81,       // a(2)
    (seq_t)289,      // a(3)
    (seq_t)961,      // a(4)
    (seq_t)3249,     // a(5)
    (seq_t)11025,    // a(6)
    (seq_t)37249     // a(7)
};

/* Returns the n-th term (1‑based) of A208035 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    computed = 0;   /* maximum index that has been computed */

    /* Ensure the array is large enough. */
    ensure_cache(n, &cache, &cap);

    /* Compute any missing values up to n. */
    if (computed < n) {
        idx_t start = computed + 1;   /* start from the next uncomputed index */
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = base[i];
            } else {
                cache[i] = 2*cache[i-1] + 3*cache[i-2] + 6*cache[i-3]
                           - cache[i-4] - cache[i-6];
            }
        }
        computed = n;
    }

    return cache[n];
}
