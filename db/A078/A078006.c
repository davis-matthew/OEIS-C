/*
 * A078006  Expansion of (1-x)/(1-x-2*x^2-2*x^3).
 * https://oeis.org/A078006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=2, a(n)=a(n-1)+2*a(n-2)+2*a(n-3) for n≥3
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a() returns the n‑th term of sequence A078006.
 * It uses a static cache so the sequence can be requested
 * in any order.  Only C standard library functions that are
 * valid in both C11 and C++17 are used.
 *
 * seq_t and idx_t are defined by the build environment.
 */
#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array holding a(0)..a(max_computed)   */
    static size_t capacity = 0;      /* allocated size of cache                */
    static size_t max_computed = 2;  /* largest index currently computed       */

    /* initialisation ----------------------------------------------------- */
    if (cache == NULL) {
        /* start with capacity for at least 8 elements   */
        capacity = 8;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory – return 0 */

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
    }

    /* ensure cache large enough */
    if (n >= capacity) {
        size_t new_cap = capacity;
        /* grow exponentially until we have space for n */
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* out‑of‑memory – return 0 */
        cache = tmp;
        capacity = new_cap;
    }

    /* if requested value already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute all missing values up to n */
    for (size_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*a(i-2) + 2*a(i-3) */
        cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] + (seq_t)2 * cache[i-3];
    }
    max_computed = n;
    return cache[n];
}
