/*
 * A120663  Expansion of x*(67 +2476*x +38216*x^2 -124633*x^3
 *          +129444*x^4)/((1-x)*(1+x)*(1-2*x)*(1+3*x)*(1-4*x)*(1-6*x)).
 * https://oeis.org/A120663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function denominator.
   a(n) = 9*a(n-1) - 7*a(n-2) - 93*a(n-3) + 152*a(n-4) + 84*a(n-5) - 144*a(n-6)
   for n >= 6.
   Initial values:
   a(0)=0, a(1)=67, a(2)=3079, a(3)=65458, a(4)=436705, a(5)=3325420. */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;       /* memoized sequence values */
    static size_t cap = 0;            /* current allocation size */

    /* Lazy initialization */
    if (!cache) {
        cap = 16;                     /* small initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* fallback on allocation failure */

        /* initialise first 6 terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)67;
        cache[2] = (seq_t)3079;
        cache[3] = (seq_t)65458;
        cache[4] = (seq_t)436705;
        cache[5] = (seq_t)3325420;
    }

    /* Convert index to size_t for array use; safe because caller limits n to ~10000 */
    size_t i = (size_t)n;

    /* Ensure cache large enough */
    if (i >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= i) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* fallback on allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to index i if not already computed */
    if (i >= 6) {
        size_t start = 6;
        /* If cache was extended before any term beyond 5, we need to start from last known. */
        if (cap > 0 && cap < 6) start = 6;
        for (size_t k = start; k <= i; ++k) {
            /* Avoid recomputing already known values */
            if (k <= 5) continue;

            seq_t term = (seq_t)9  * cache[k - 1]
                       - (seq_t)7  * cache[k - 2]
                       - (seq_t)93 * cache[k - 3]
                       + (seq_t)152 * cache[k - 4]
                       + (seq_t)84  * cache[k - 5]
                       - (seq_t)144 * cache[k - 6];
            cache[k] = term;
        }
    }

    return cache[i];
}
