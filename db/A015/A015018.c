/*
 * A015018  q-factorial numbers for q=-5.
 * https://oeis.org/A015018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015018: q‑factorial numbers for q = -5
 * Recurrence:
 *   a(0) = 1
 *   a(1) = 1
 *   a(n) = ((-5)^n - 1) * a(n-1) / (-6)   for n ≥ 2
 *
 * The implementation caches computed terms and powers of (-5)
 * to allow out‑of‑order calls with O(1) per new n.
 */

#include <stdlib.h>

/* Compute sequence value */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cached   = NULL;  /* array of a(k) for k = 0 … cache_len-1 */
    static idx_t  cache_len = 0;    /* number of elements stored */

    /* Cache of current power (-5)^k and its exponent k */
    static seq_t pow_cur   = 1; /* (-5)^0 = 1 initially */
    static idx_t pow_exp   = 0; /* exponent corresponding to pow_cur */

    /* Ensure we have space for requested index */
    if (cache_len == 0) {               /* first call */
        cached = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!cached) return 0;          /* fallback on allocation failure */
        cached[0] = (seq_t)1;           /* a(0) = 1 */
        cache_len = 1;
    }

    if (n >= cache_len) {
        /* Expand cache to accommodate up to n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cached = tmp;

        /* Compute missing terms incrementally */
        for (idx_t k = cache_len; k <= n; ++k) {
            /* Update power: pow_cur *= (-5); pow_exp++ */
            pow_cur = pow_cur * (seq_t)(-5);
            ++pow_exp;

            /* term = ((-5)^k - 1) / (-6) */
            seq_t term = (pow_cur - (seq_t)1) / (seq_t)(-6);

            /* a(k) = term * a(k-1) */
            seq_t val = term * cached[k - 1];
            cached[k] = val;
        }
        cache_len = new_cap;
    }

    return cached[n];
}
