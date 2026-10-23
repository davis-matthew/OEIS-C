/*
 * A184001  Number of (n+1) X 8 0..2 arrays with all 2 X 2 subblock sums the
 *          same.
 * https://oeis.org/A184001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184001
   Formula: a(n) = 6*a(n-1) - 60*a(n-3) + 85*a(n-4) + 150*a(n-5)
              - 360*a(n-6) + 396*a(n-8) - 216*a(n-9)
   Initial values (offset 1):
   a(1)=13225, a(2)=15555, a(3)=19217, a(4)=26655, a(5)=39697,
   a(6)=66591, a(7)=118097, a(8)=227319, a(9)=453025 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle offset 1: a(1) is first term. For n<1 behavior is undefined. */
    if (n == 0)
        return (seq_t)0;

    /* Initial terms */
    static const seq_t init[9] = {
        (seq_t)13225,  (seq_t)15555,  (seq_t)19217,
        (seq_t)26655,  (seq_t)39697,  (seq_t)66591,
        (seq_t)118097, (seq_t)227319, (seq_t)453025
    };

    /* Static cache to memoize computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity in elements */
    static size_t len = 0;   /* number of filled elements (max index+1) */

    /* Initial allocation and filling of known values */
    if (!cache) {
        /* Allocate at least as many as required for the first 9 terms */
        size_t req = 9;
        if (n > req)
            req = n;
        /* Grow capacity to a power of two for efficiency */
        size_t newcap = 16;
        while (newcap < req)
            newcap <<= 1;

        cache = (seq_t *)malloc(sizeof(seq_t) * newcap);
        if (!cache) return (seq_t)0;        /* Out of memory – unlikely in tests */
        cap = newcap;
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        len = 9;
    }

    /* Ensure the array is large enough for the requested index */
    if ((size_t)n > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;         /* Allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* If we already know the value, return it */
    if (n <= len)
        return cache[n - 1];

    /* Compute terms from len+1 up to n using the linear recurrence */
    for (size_t i = len; i < (size_t)n; ++i) {
        /* Current n (1‑based) */
        idx_t idx = (idx_t)(i + 1);

        /* Positive contributions */
        seq_t pos = cache[idx - 2] * ((seq_t)6);          /* a(n-1) */
        pos += cache[idx - 5] * ((seq_t)85);             /* a(n-4) */
        pos += cache[idx - 6] * ((seq_t)150);            /* a(n-5) */
        pos += cache[idx - 9] * ((seq_t)396);            /* a(n-8) */

        /* Negative contributions */
        seq_t neg = cache[idx - 4] * ((seq_t)60);         /* a(n-3) */
        neg += cache[idx - 7] * ((seq_t)360);            /* a(n-6) */
        neg += cache[idx - 10] * ((seq_t)216);           /* a(n-9) */

        /* Result (positive sums are guaranteed to be >= negative sums) */
        cache[i] = pos - neg;
    }

    len = n;
    return cache[n - 1];
}
