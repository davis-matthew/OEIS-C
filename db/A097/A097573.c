/*
 * A097573  Denominator of f(n) where f(1) = 1, f(n+1) = n^2/f(n) + f(n)/n^2 +
 *          2.
 * https://oeis.org/A097573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A097573 – denominator of f(n) where  
    f(1) = 1,  f(n+1) = n^2/f(n) + f(n)/n^2 + 2 */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static cache of previously computed denominators */
    static seq_t *denom = NULL;         /* array of denominators a(1..current_n)  */
    static size_t denom_cap = 0;        /* capacity of the array                 */
    static idx_t current_n = 1;         /* maximum index that has already been computed */
    static seq_t p = 1;                 /* numerator of f(current_n)             */
    static seq_t q = 1;                 /* denominator of f(current_n)           */

    /* initialise on first call */
    if (!denom) {
        denom_cap = 4;                       /* arbitrary small start size          */
        denom = (seq_t*)malloc(denom_cap * sizeof(seq_t));
        if (!denom) return 0;                /* out‑of‑memory → 0 (should not happen) */
        denom[1] = 1;                        /* a(1) = 1                              */
    }

    /* extend array if necessary */
    while ((size_t)n >= denom_cap) {
        size_t new_cap = denom_cap * 2;
        if ((size_t)n + 1 > new_cap) new_cap = n + 1;
        denom = (seq_t*)realloc(denom, new_cap * sizeof(seq_t));
        denom_cap = new_cap;
    }

    /* iterate until we have computed a(n) */
    while (current_n < n) {
        seq_t n_seq = (seq_t)current_n;      /* the n used in the recurrence */
        seq_t n_sq  = n_seq * n_seq;         /* n^2 */
        seq_t temp  = n_sq * q + p;          /* n^2*q + p  */
        seq_t p_new = temp * temp;           /* (n^2*q + p)^2 */
        seq_t q_new = p * n_sq * q;          /* p * n^2 * q   */
        seq_t g     = gcd(p_new, q_new);     /* reduce fraction */
        p_new = p_new / g;
        q_new = q_new / g;
        ++current_n;                         /* now f(current_n) is for index current_n */
        denom[current_n] = q_new;            /* store denominator a(current_n) */
        p = p_new;
        q = q_new;
    }
    return denom[n];
}
