/*
 * A203239  Odd numbered terms of the sequence whose n-th term is the (n-1)-st
 *          elementary symmetric function of (i, 2i, 3i, ..., ni), where
 *          i=sqrt(-1).
 * https://oeis.org/A203239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1)*(2*n)! * Sum_{i=1..2n} 1/i
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static tables
         fac[i] = i!            (i up to 2*n)
         T[i]  = i! * H_i       (H_i harmonic sum)
    */
    static seq_t *fac = NULL;
    static seq_t *T   = NULL;
    static idx_t max_used = 0;   /* largest m for which fac and T are initialized */
    static size_t alloc_cap = 0; /* current allocated array size (>= max_used+1) */

    /* Needed range is from 0 to 2*n inclusive. */
    idx_t limit = 2 * n;
    if (alloc_cap < (size_t)(limit + 1)) {
        /* Grow capacity: at least double, but at least limit+1. */
        size_t new_cap = alloc_cap > 0 ? alloc_cap * 2 : 16;
        if (new_cap < (size_t)(limit + 1))
            new_cap = (size_t)(limit + 1);
        seq_t *new_fac = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
        seq_t *new_T   = (seq_t *)realloc(T,   new_cap * sizeof(seq_t));
        if (!new_fac || !new_T)
            abort();   /* memory exhausted */

        fac = new_fac;
        T   = new_T;
        alloc_cap = new_cap;

        /* Initialise the zero‑th entries if this is the very first allocation. */
        if (max_used == 0) {
            fac[0] = 1;
            T[0]   = 0;
        }
    }

    /* Build tables up to 'limit' if required. */
    for (idx_t i = max_used + 1; i <= limit; ++i) {
        fac[i] = fac[i - 1] * (seq_t)i;          /* i! = (i-1)! * i */
        T[i]   = (seq_t)i * T[i - 1] + fac[i - 1];/* T_i = i*T_{i-1} + (i-1)! */
    }
    max_used = limit;

    seq_t result = T[limit];
    /* Apply the sign factor (-1)^(n+1). */
    if ((n & 1U) == 0)   /* n even -> negative sign */
        result = -result;

    return result;
}
