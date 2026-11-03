/*
 * A208219  a(n)=(a(n-1)^3*a(n-3)+a(n-2))/a(n-4) with a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A208219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^3 * a(n-3) + a(n-2)) / a(n-4) with initial values
 *            a(0) = a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms   */
    static size_t capacity = 0;      /* allocated array size      */
    static size_t used = 0;          /* number of terms computed */

    /* Ensure cache can hold up to index n */
    if ((idx_t)used <= n) {
        idx_t new_cap = n + 1;                      /* capacity needed */
        seq_t *tmp = seq_realloc(cache, capacity, new_cap);
        if (!tmp) abort();                         /* out of memory */
        cache = tmp;
        capacity = (size_t)new_cap;

        /* Zero‑initialize new elements (trivial for built‑ins, ctor for arbitrary types) */
        for (size_t i = used; i < capacity; ++i) {
            cache[i] = (seq_t)0;
        }

        /* Compute missing terms */
        for (size_t i = used; i <= (size_t)n; ++i) {
            if (i < 4) {                      /* initial values */
                cache[i] = (seq_t)1;
                continue;
            }
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];

            seq_t prod = t1;
            prod *= t1;      /* t1^2  */
            prod *= t1;      /* t1^3  */
            prod *= t3;      /* t1^3 * t3 */
            prod += t2;      /* + t2  */
            cache[i] = prod / t4; /* exact division */
        }
        used = (size_t)n + 1;                  /* now n is computed */
    }

    return cache[(size_t)n];
}
