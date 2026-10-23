/*
 * A183357  One quarter the number of n X 5 1..4 arrays with no two neighbors
 *          of any element equal to each other.
 * https://oeis.org/A183357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4) for n > 7, * initial values:
 *            a(1)=108, a(2)=2304, a(3)=3888, * a(4)=9216, a(5)=24192,
 *            a(6)=57600, a(7)=137088
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 43, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* The function must have external linkage as specified */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* Sequence is defined for n ≥ 1 */

    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t last_computed = 0;  /* highest n for which cache[n-1] is valid */

    /* Base values */
    static const seq_t base[7] = {
        108ULL,      /* a(1) */
        2304ULL,     /* a(2) */
        3888ULL,     /* a(3) */
        9216ULL,     /* a(4) */
        24192ULL,    /* a(5) */
        57600ULL,    /* a(6) */
        137088ULL    /* a(7) */
    };

    /* Ensure the cache is large enough */
    if ((size_t)n > capacity) {
        size_t new_capacity = (size_t)n * 2;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_capacity);
        if (new_cache == NULL)
            return 0; /* Out of memory: fallback to 0 */
        if (cache != NULL) {
            memcpy(new_cache, cache, sizeof(seq_t) * capacity);
            free(cache);
        }
        cache = new_cache;
        capacity = new_capacity;
    }

    /* If the requested term is already cached, return it */
    if (n <= (idx_t)last_computed)
        return cache[n - 1];

    /* Fill missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 7) {
            val = base[i - 1];
        } else {
            /* Recurrence: a(i) = 2*a(i-1) + 2*a(i-3) + a(i-4) */
            seq_t t1 = cache[i - 2]; /* a(i-1) */
            seq_t t2 = cache[i - 4]; /* a(i-3) */
            seq_t t3 = cache[i - 5]; /* a(i-4) */
            val = 2 * t1 + 2 * t2 + t3;
        }
        cache[i - 1] = val;
    }

    last_computed = n;
    return cache[n - 1];
}
