/*
 * A160569  a(n)=9*(a(n-1)a(n-3)-a(n-2)^2)/a(n-4), a(1)=a(2)=a(3)=1, a(4)=-9.
 * https://oeis.org/A160569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 11, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*(a(n-1)*a(n-3)-a(n-2)^2)/a(n-4) for n>=4,
   with a(0)=1, a(1)=1, a(2)=1, a(3)=-9. */
#include <stdlib.h>
#include <stddef.h>

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
/* C mode: use __int128 to avoid overflow for 64‑bit seq_t. */
static inline seq_t recurrence_term(seq_t a_nm1, seq_t a_nm3,
                                    seq_t a_nm2, seq_t a_nm4)
{
    __int128 num = (__int128)a_nm1 * a_nm3 - (__int128)a_nm2 * a_nm2;
    num *= 9;
    return (seq_t)(num / a_nm4);
}
#else
/* C++ mode or C without __int128 support.  seq_t may be a big‑int class
   with overloaded +,-,*,/.  The expression is performed in that type. */
static inline seq_t recurrence_term(seq_t a_nm1, seq_t a_nm3,
                                    seq_t a_nm2, seq_t a_nm4)
{
    return (a_nm1 * a_nm3 - a_nm2 * a_nm2) * 9 / a_nm4;
}
#endif

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0; /* number of elements stored so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* out of memory – unlikely in test harness */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-9;
        computed = 4;
    }

    /* grow cache if needed */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return precomputed values */
    if (n < computed)
        return cache[(size_t)n];

    /* compute terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        cache[i] = recurrence_term(cache[i-1], cache[i-3],
                                   cache[i-2], cache[i-4]);
    }
    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
