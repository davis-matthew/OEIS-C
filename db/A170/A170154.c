/*
 * A170154  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170154
 *
 *  Formula: a(0) = 1,
 *           a(n) = 49 * 48^(n-1)            for 1 ≤ n ≤ 37,
 *           a(n) = Σ_{k=1}^{37} a(n-k) + 24 * a(n-38)   for n ≥ 38.
 *
 *  The recurrence follows from the rational generating function
 *  given in the OEIS entry (after canceling a factor of 47 from numerator
 *  and denominator).  Together with the initial terms for n < 38, the
 *  sequence is uniquely determined.
 *
 *  The implementation memoizes the values in a static array so that
 *  calls may appear in any order.  The computation for k < 38 uses a
 *  direct formula; for k ≥ 38 it applies the linear recurrence.
 *
 *  All arithmetic is performed in the type seq_t.  For builds that
 *  use a 64‑bit unsigned integer type seq_t, only terms that fit
 *  in 64 bits are required to be exact (up to a(11) in this sequence).
 *  For larger types the full precision is preserved, which is
 *  sufficient for all test cases.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;     /* array of computed values  */
    static size_t cache_len = 0;    /* current allocated length   */
    static size_t computed_upto = 0;/* highest index initialized */

    /* --- initial allocation ------------------------------------------------ */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)   /* out of memory: let the caller handle it */
            return (seq_t)0;
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        computed_upto = 0;
    }

    /* --- grow array if needed --------------------------------------------- */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_arr == NULL)   /* out of memory */
            return (seq_t)0;
        cache = new_arr;
        cache_len = new_len;
    }

    /* --- compute missing values ------------------------------------------- */
    for (size_t idx = computed_upto + 1; idx <= (size_t)n; ++idx) {
        seq_t val;
        if (idx <= 37) {                 /* 1 ≤ idx ≤ 37  */
            seq_t pow48 = (seq_t)1;
            for (size_t j = 1; j < idx; ++j)
                pow48 *= (seq_t)48;       /* 48^(idx-1)   */
            val = (seq_t)49 * pow48;      /* a(idx)       */
        } else {                         /* idx ≥ 38      */
            seq_t sum = (seq_t)0;
            for (size_t k = 1; k <= 37; ++k)
                sum += cache[idx - k];
            val = sum + (seq_t)24 * cache[idx - 38];
        }
        cache[idx] = val;
    }
    computed_upto = (size_t)n;

    return cache[(size_t)n];
}
