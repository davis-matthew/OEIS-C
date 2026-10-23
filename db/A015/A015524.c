/*
 * A015524  a(n) = 3*a(n-1) + 7*a(n-2), with a(0) = 0, a(1) = 1.
 * https://oeis.org/A015524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc/realloc */

/* Memoized iterative computation of A015524 */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* static cache for already computed terms */
    static seq_t *cache   = NULL;
    static size_t cap     = 0; /* capacity in elements */
    static idx_t  sz      = 0; /* number of terms stored (>= cap ? not necessarily) */

    /* Allocate or grow cache if needed */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort or handle as desired */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms */
    if (sz <= n) {
        /* Ensure base values are available */
        if (sz == 0) { cache[0] = (seq_t)0; sz = 1; }
        if (sz == 1) { cache[1] = (seq_t)1; sz = 2; }

        for (idx_t i = sz; i <= n; ++i) {
            cache[i] = (seq_t)( (seq_t)3 * cache[i-1] + (seq_t)7 * cache[i-2] );
        }
        sz = n + 1;
    }

    return cache[n];
}
