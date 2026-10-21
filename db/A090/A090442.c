/*
 * A090442  Row sums of array A090452 (s2_{3,2}, scaled (3,2)-Stirling2).
 * https://oeis.org/A090442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*(2*n-1)*a(n-1) - 4*(n-2)*a(n-2))/(n+1), a(1)=1, a(2)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _SEQ_FUNC_H_
#define _SEQ_FUNC_H_
#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *table = NULL;
    static idx_t capacity = 0;     /* allocated size of array (>= n+1) */
    static idx_t max_cached = 0;   /* largest n that has been computed, 0 means none */

    /* ensure the table can hold indices up to n */
    if (capacity <= n) {
        idx_t new_cap = capacity ? capacity * 2 : 8;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory – return 0 */
        table = tmp;
        capacity = new_cap;
    }

    /* initialise base terms if needed */
    if (max_cached < 2) {
        table[0] = (seq_t)0;          /* unused but convenient */
        table[1] = (seq_t)1;
        table[2] = (seq_t)6;
        max_cached = 2;
    }

    /* compute values up to n if necessary */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* we know that a(1)=1, a(2)=6 have been set above */
#ifdef __cplusplus
        /* In C++ we can distinguish integral types from big‑integer classes */
        if constexpr (std::is_integral<seq_t>::value) {
            /* Use 128‑bit intermediates to avoid overflow on integral types */
            unsigned __int128 mult1 = (unsigned __int128)6 *
                                     (unsigned __int128)(2ULL*i - 1ULL);
            unsigned __int128 mult2 = (unsigned __int128)4 *
                                     (unsigned __int128)(i - 2ULL);
            unsigned __int128 t1 = (unsigned __int128)table[i-1] * mult1;
            unsigned __int128 t2 = (unsigned __int128)table[i-2] * mult2;
            unsigned __int128 num = t1 - t2;
            unsigned __int128 den = (unsigned __int128)(i + 1ULL);
            table[i] = (seq_t)(num / den);
        } else {
            /* For big‑integer types use the natural arithmetic of the class */
            uint64_t mult1 = 6ULL * (2ULL*i - 1ULL);
            uint64_t mult2 = 4ULL * (i - 2ULL);
            seq_t t1 = table[i-1] * (seq_t)mult1;
            seq_t t2 = table[i-2] * (seq_t)mult2;
            seq_t num = t1 - t2;
            table[i] = num / (seq_t)(i + 1ULL);
        }
#else
        /* Pure C implementation – rely on unsigned __int128 for the intermediate */
        unsigned __int128 mult1 = (unsigned __int128)6 *
                                 (unsigned __int128)(2ULL*i - 1ULL);
        unsigned __int128 mult2 = (unsigned __int128)4 *
                                 (unsigned __int128)(i - 2ULL);
        unsigned __int128 t1 = (unsigned __int128)table[i-1] * mult1;
        unsigned __int128 t2 = (unsigned __int128)table[i-2] * mult2;
        unsigned __int128 num = t1 - t2;
        unsigned __int128 den = (unsigned __int128)(i + 1ULL);
        table[i] = (seq_t)(num / den);
#endif
    }

    max_cached = n;
    return table[n];
}
#endif /* _SEQ_FUNC_H_ */
