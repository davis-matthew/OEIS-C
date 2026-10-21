/*
 * A203293  Number of arrays of 6 nondecreasing integers in -n..n with sum
 *          zero and equal numbers greater than zero and less than zero.
 * https://oeis.org/A203293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 3*a(n-1) - a(n-2) -
 *            4*a(n-3) + 2*a(n-4) + 2*a(n-5) + 2*a(n-6) - 4*a(n-7) - a(n-8) +
 *            3*a(n-9) - a(n-10) with initial values a(1) = 4, a(2) = 10, a(3)
 *            = 28, a(4) = 69, a(5) = 154, a(6) = 310, a(7) = 580, a(8) =
 *            1013, a(9) = 1680, a(10) = 2662
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 193, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Function declared externally by the build system */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;   /* number of terms currently cached */
    static idx_t capacity = 0; /* allocated capacity */

    /* Initial terms for n = 1..10 */
    static const seq_t init[10] = {
        (seq_t)4, (seq_t)10, (seq_t)28, (seq_t)69, (seq_t)154,
        (seq_t)310, (seq_t)580, (seq_t)1013, (seq_t)1680, (seq_t)2662
    };

    /* Ensure we have at least the first 10 terms cached */
    if (cache == NULL) {
        capacity = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        size = 10;
    }

    /* Return immediately if we already cached the requested term */
    if (n <= size)
        return cache[n - 1];

    /* Grow the cache if necessary -----------------------------------------------
     * We grow the capacity by doubling it until it fits the new size.
     */
    if ((idx_t)capacity < n) {
        idx_t new_cap = capacity;
        while (new_cap < n) {
            if (new_cap == 0)
                new_cap = 1;
            new_cap *= 2;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) {
            /* Allocation failed; return 0 as defensive fallback */
            return (seq_t)0;
        }
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute missing terms up to n -------------------------------------------
     * The recurrence uses a(n-1) ... a(n-10).  All indices are 0‑based here,
     * so a(n) is stored at cache[n-1].
     */
    for (idx_t i = size; i < n; ++i) {
        /* Retrieve the needed previous terms */
        seq_t a1  = cache[i - 1];   /* a(n-1) */
        seq_t a2  = cache[i - 2];   /* a(n-2) */
        seq_t a3  = cache[i - 3];   /* a(n-3) */
        seq_t a4  = cache[i - 4];   /* a(n-4) */
        seq_t a5  = cache[i - 5];   /* a(n-5) */
        seq_t a6  = cache[i - 6];   /* a(n-6) */
        seq_t a7  = cache[i - 7];   /* a(n-7) */
        seq_t a8  = cache[i - 8];   /* a(n-8) */
        seq_t a9  = cache[i - 9];   /* a(n-9) */
        seq_t a10 = cache[i -10];  /* a(n-10) */

        /* Positive part of the recurrence */
        seq_t pos = 3 * a1 + 2 * a4 + 2 * a5 + 2 * a6 + 3 * a9;

        /* Negative part of the recurrence */
        seq_t neg = a2 + 4 * a3 + 4 * a7 + a8 + a10;

        /* The recurrence guarantees pos >= neg, so subtraction is safe */
        cache[i] = pos - neg;
    }

    size = n;  /* we now have all terms up to n */
    return cache[n - 1];
}
