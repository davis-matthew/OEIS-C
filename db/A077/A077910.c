/*
 * A077910  Expansion of 1/((1-x)*(1+x+2*x^2-2*x^3)).
 * https://oeis.org/A077910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 39, i64 n <= 78, i128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A077910: Expansion of 1/((1-x)*(1+x+2*x^2-2*x^3))
 *
 * Recurrence:  a(0)=1, a(1)=0, a(2)=-1, a(3)=4
 *              a(n) = -a(n-2) + 4*a(n-3) - 2*a(n-4)   for n ≥ 4
 *
 * The implementation uses a self‑resizing static cache so that calls may
 * arrive in any order.  The code is valid both in C11 and in C++17; when
 * compiled as C++ the constructor for `seq_t` objects is invoked by
 * placement `new`.  In plain C no construction is necessary because
 * `seq_t` is an integer type.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>   /* placement new */
#endif

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* array of terms           */
    static idx_t cache_cap = 0;          /* allocated capacity       */
    static idx_t next_computed = 0;      /* next index to compute    */

    /* --- initialise the cache if needed -------------------------------- */

    if (cache == NULL) {
        /* allocate initial capacity for the first 4 terms */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure → undefined */

        /* construct elements when compiled as C++ */
#ifdef __cplusplus
        for (idx_t i = 0; i < cache_cap; ++i)
            ::new((void *)(&cache[i])) seq_t();
#endif

        /* set the known initial values */
        cache[0] =  (seq_t) 1;
        cache[1] =  (seq_t) 0;
        cache[2] = (seq_t)-1;
        cache[3] =  (seq_t) 4;

        next_computed = 4;                   /* indices [0,3] are ready   */
    }

    /* --- ensure capacity for requested index --------------------------- */

    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        /* grow until new_cap > n */
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure → undefined */
        cache = tmp;

        /* construct newly allocated elements when compiled as C++ */
#ifdef __cplusplus
        for (idx_t i = cache_cap; i < new_cap; ++i)
            ::new((void *)(&cache[i])) seq_t();
#endif

        cache_cap = new_cap;
    }

    /* --- compute terms up to n if they are not yet known --------------- */

    if (n >= next_computed) {
        for (idx_t i = next_computed; i <= n; ++i) {
            /* recurrence uses values already stored */
            cache[i] = -cache[i-2] + (seq_t)4 * cache[i-3] - (seq_t)2 * cache[i-4];
        }
        next_computed = n + 1;
    }

    return cache[n];
}
