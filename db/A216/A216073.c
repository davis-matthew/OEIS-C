/*
 * A216073  The list of the a(n)-values in the common solutions to k+1=b^2 and
 *          6*k+1=a^2.
 * https://oeis.org/A216073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-2) - a(n-4)    (n ≥ 5)
 * Initial values:  a(1)=1, a(2)=7, a(3)=17, a(4)=71
 */
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* outside the offset */

    /* static cache for memoisation (1‑based index -> 0‑based array) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated capacity */
    static size_t len   = 0;  /* number of terms stored */

    /* initialise cache on first call */
    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)71;
        len = 4;
    }

    /* grow cache if needed */
    if ((size_t)n > len) {
        size_t required = (size_t)n;
        if (required > cap) {
            size_t new_cap = cap ? cap << 1 : 4;
            while (new_cap < required)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation error */
            cache = tmp;
            cap   = new_cap;
        }
        /* compute remaining terms up to n (0-based index n‑1) */
        for (idx_t i = (idx_t)len; i < n; ++i) {
            /* recurrence holds for i ≥ 4 (a(5) onward) */
            /* cast i to size_t for array indices */
            cache[(size_t)i] = (seq_t)10 * cache[(size_t)(i - 2)]
                               - cache[(size_t)(i - 4)];
        }
        len = required;
    }

    return cache[(size_t)(n - 1)];
}
