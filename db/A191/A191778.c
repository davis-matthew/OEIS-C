/*
 * A191778  a(1) = 1; a(n)= 2*lcm(n, a(n - 1)).
 * https://oeis.org/A191778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 2 * lcm(n, a(n-1)) for n > 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   _seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/_seq_gcd.h"

/* Local Euclidean gcd – works for all seq_t types that support % and != 0 */

/* Static cache for memoization: 1‑based indexing */
static seq_t *cache = NULL;          /* pointer to cached terms */
static idx_t cached = 0;             /* largest index already computed */

/* ------------------------------------------------------------
 *  Compute a(n)
 * ------------------------------------------------------------
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the OEIS sequence */

    /* First invocation: allocate and initialise the cache.   */
    if (cache == NULL) {
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
        cache[0] = (seq_t)0;               /* unused slot */
        cache[1] = (seq_t)1;               /* a(1) = 1 */
        cached = 1;
    }

    /* Expand cache if necessary and compute missing terms.  */
    if (cached < n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t g   = _seq_gcd(prev, (seq_t)i);
            seq_t lcm = (prev / g) * (seq_t)i;
            cache[i] = lcm << 1;           /* 2 * lcm */
        }
        cached = n;
    }

    return cache[n];
}
