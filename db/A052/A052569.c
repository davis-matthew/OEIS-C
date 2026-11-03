/*
 * A052569  E.g.f. 1/((1-x)(1-x^3)).
 * https://oeis.org/A052569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * floor((n+3)/3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 443 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Memoized factorial table */
    static seq_t *fact      = NULL;   /* array of factorials */
    static idx_t   fact_cap = 0;      /* allocated capacity */
    static idx_t   fact_len = 0;      /* number of precomputed factorials (= current size) */

    /* Initial allocation */
    if (fact == NULL) {
        fact_cap = 16;                            /* arbitrary small start */
        fact = mem_new(seq_t, fact_cap);
        fact[0] = (seq_t)1;                       /* 0! = 1 */
        fact_len = 1;
    }

    /* Grow allocation if needed */
    if (n >= fact_cap) {
        /* double until enough */
        while (n >= fact_cap) fact_cap <<= 1;
        fact = mem_resize(seq_t, fact, fact_cap);
    }

    /* Compute missing factorials */
    for (idx_t i = fact_len; i <= n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_len = n + 1;

    /* a(n) = n! * floor((n+3)/3) */
    seq_t mult = (seq_t)((n + 3) / 3);
    return fact[n] * mult;
}
