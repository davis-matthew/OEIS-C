/*
 * A083709  G.f.: (1-x+2*x^2+2*x^3+2*x^4-x^5+x^6)/((1-x)*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A083709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)-a(n-5)+2*a(n-6)+a(n-7)-a(n-8);
 *            a(0)=1, a(1)=0, a(2)=4, a(3)=5, a(4)=13, a(5)=17, a(6)=33,
 *            a(7)=41
 * Range:     u8 n <= 13, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed values          */
    static size_t cap   = 0;      /* current capacity of cache         */
    static idx_t last   = (idx_t)-1; /* index of last computed value */

    /* Initialize cache on first call or when expanding */
    if (cache == NULL) {
        /* Allocate space for at least the first 8 terms or up to n */
        cap = n + 1 > 8 ? (size_t)(n + 1) : 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* Base values a(0) … a(7) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)17;
        cache[6] = (seq_t)33;
        cache[7] = (seq_t)41;
        last   = 7;
    } else if ((size_t)n + 1 > cap) {
        /* Grow cache to accommodate requested n */
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms up to n if not already computed */
    if (last < n) {
        idx_t start = last + 1;
        if (start < 8) start = 8;  /* base terms already known */
        for (idx_t i = start; i <= n; ++i) {
            seq_t t = cache[i - 1];
            t += cache[i - 2] * 2U;
            t -= cache[i - 3];
            t -= cache[i - 4] * 2U;
            t -= cache[i - 5];
            t += cache[i - 6] * 2U;
            t += cache[i - 7];
            t -= cache[i - 8];
            cache[i] = t;
        }
        last = n;
    }

    return cache[n];
}
