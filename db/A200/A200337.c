/*
 * A200337  a(n+1) = Sum_{k=0..n, n XOR k <= n} a(k)*a(n XOR k) for n>=0 with
 *          a(0)=1.
 * https://oeis.org/A200337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   for n ≥ 0:
     a(n+1) = Σ_{k=0..n  ,  n XOR k ≤ n} a(k) * a(n XOR k)
*/
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Helper to create an array of seq_t ------------------------------------------------- */
static inline seq_t *create_array(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* Helper to reallocate an array of seq_t -------------------------------------------- */
static inline seq_t *realloc_array(seq_t *old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) {
        tmp[i] = old[i];            /* copy-construct/move each element           */
    }
    delete [] old;
    return tmp;
#else
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* static cache for memoisation ----------------------------------- */
    static idx_t      max_computed = 0;  /* largest n already computed    */
    static seq_t      *cache       = NULL;  /* cache[0..max_computed]       */
    static size_t     cache_cap   = 0;       /* allocated capacity           */

    /* allocate cache lazily on first call ----------------------------- */
    if (!cache) {
        cache_cap = 1;
        cache     = create_array(cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)1;     /* a(0)=1 */
        max_computed = 0;
    }

    /* if value already known, return it ------------------------------- */
    if (n <= max_computed)
        return cache[n];

    /* grow cache if needed -------------------------------------------- */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = realloc_array(cache, cache_cap, new_cap);
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n -------------------------------- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t m   = i - 1;                 /* n in the recurrence for a(i)     */
        for (idx_t k = 0; k <= m; ++k) {
            idx_t x = m ^ k;
            if (x <= m)
                sum += cache[k] * cache[x];
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
