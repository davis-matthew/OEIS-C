/*
 * A138523  a(n) = Sum_{k=1..n} (2k-1)!.
 * https://oeis.org/A138523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (2k-1)!; Recurrence: a(1)=1, a(2)=7, a(n) =
 *            (4n^2-6n+3)·a(n-1) - 2(n-1)(2n-1)·a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc/realloc, abort */

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocation capacity (size of array) */
    static idx_t max_computed = 0;/* highest index already computed */

    /* Expand the cache up to index n (inclusive) if needed */
    if (n + 1 > cache_cap) {            /* +1 because we use 1‑based indices */
        idx_t new_cap = (cache_cap == 0) ? 3 : cache_cap;
        while (new_cap <= n)
            new_cap *= 2;

#ifdef __cplusplus
        /* In C++ new[] constructs the objects; we must copy the old data. */
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (idx_t i = 0; i <= max_computed; ++i)
                tmp[i] = cache[i];
            delete[] cache;            /* destroys old objects */
        }
        cache = tmp;
#else
        /* In C realloc will copy the contents for us. */
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failure */
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* Initialise the base terms if required */
    if (max_computed < 1 && n >= 1) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }
    if (max_computed < 2 && n >= 2) {
        cache[2] = (seq_t)7;
        max_computed = 2;
    }

    /* Return pre‑computed value if already known */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t coeff1 = (seq_t)4 * ii * ii - (seq_t)6 * ii + (seq_t)3;
        seq_t coeff2 = (seq_t)2 * (seq_t)(i - 1) *
                       ((seq_t)2 * ii - (seq_t)1);
        cache[i] = coeff1 * cache[i - 1] - coeff2 * cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
