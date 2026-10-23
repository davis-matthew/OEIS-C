/*
 * A041313  Denominators of continued fraction convergents to sqrt(170).
 * https://oeis.org/A041313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 26*a(n-1) + a(n-2), a(0)=1, a(1)=26
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A041313: denominators of continued‑fraction
 * convergents to sqrt(170), which satisfies a(0)=1, a(1)=26,
 * a(n)=26*a(n-1)+a(n-2) for n>=2.  The result is returned as a value of type
 * seq_t.  The function is memoised so that repeated calls with arbitrary
 * indices are fast.  The implementation works for any integer type seq_t
 * that supports the usual arithmetic operators.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache      = NULL;   /* pointer to array of seq_t values */
    static size_t cap        = 0;     /* allocated capacity of cache       */
    static size_t computed   = 0;     /* number of terms actually computed */

    /* Initialise the cache on first call.  Two terms are enough to start. */
    if (cache == NULL) {
        cap = 4;                       /* initial capacity (will grow as needed) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        computed = 2;
    }

    /* Ensure the cache can hold term n (index n). */
    if (n + 1 > computed) {
        /* Grow the allocation if necessary. */
        if (n + 1 > cap) {
            size_t newcap = cap * 2;
            while (newcap < (size_t)(n + 1)) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cap = newcap;
        }
        /* Compute missing values using the recurrence. */
        for (size_t i = computed; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)26 * cache[i - 1] + cache[i - 2];
        }
        computed = n + 1;
    }

    return cache[(size_t)n];
}
