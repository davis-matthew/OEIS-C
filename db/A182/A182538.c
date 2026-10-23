/*
 * A182538  a(n) = (a(n-1) AND a(n-2)) + n.
 * https://oeis.org/A182538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = (a(n-1) & a(n-2)) + n
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with initial values a(0)=0, a(1)=1.  Cache results for fast reuse. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache storage */
    static seq_t *cache    = NULL;  /* array of calculated terms */
    static size_t cap     = 0;     /* allocated capacity */
    static size_t count   = 0;     /* number of terms stored (N+1) */

    /* Grow cache if needed */
    if (n >= cap) {
        size_t new_cap = (n + 1) * 2;          /* double to keep growth amortized */
        if (new_cap < n + 1) new_cap = n + 1; /* guard against overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure; caller should handle */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n >= count) {
        /* Ensure base cases are in place */
        if (count == 0) {
            cache[0] = (seq_t)0;
            count = 1;
        }
        if (count == 1) {
            cache[1] = (seq_t)1;
            count = 2;
        }

        for (size_t i = count; i <= (size_t)n; ++i) {
            seq_t val = cache[i - 1] & cache[i - 2];
            val += (seq_t)i;
            cache[i] = val;
        }
        count = n + 1;
    }

    return cache[n];
}
