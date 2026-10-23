/*
 * A169493  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169493
   a(n) satisfies:  a_n = 46·(a_{n-1}+…+a_{n-32}) – 1081·a_{n-33} + p_n
   where p_0 = 1, p_1 … p_32 = 2, p_33 = 1, p_n = 0 otherwise.  */

#include <stddef.h>     /* size_t */
#include <stdbool.h>    /* bool */
#include <stdlib.h>     /* realloc, NULL */
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t, unsigned int); */

seq_t a(idx_t n)
{
    /* static cache for memoization ------------------------------------- */
    static seq_t *cache     = NULL;   /* array of computed terms          */
    static size_t cap       = 0;      /* allocated size (number of elems)*/
    static idx_t last_computed = 0;   /* highest index computed so far     */
    static bool  initialized   = false;

    /* Allocate or resize cache if necessary ----------------------------- */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) {        /* allocation failed – return 0 as fallback   */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize a(0) on the first call ------------------------------- */
    if (!initialized) {
        cache[0] = (seq_t)1;
        last_computed = 0;
        initialized = true;
    }

    /* Compute all terms up to n (if not already done) ------------------- */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t term_p;   /* p_k coefficient from the numerator   */
        if (k == 0)      term_p = (seq_t)1;
        else if (k <= 32) term_p = (seq_t)2;
        else if (k == 33) term_p = (seq_t)1;
        else             term_p = (seq_t)0;

        /* 46 * sum_{i=1}^{32} a_{k-i}  */
        seq_t sum = (seq_t)0;
        size_t lim = (k < 32) ? (size_t)k : 32;
        for (size_t i = 1; i <= lim; ++i) {
            sum += mul_small(cache[k - i], 46);
        }

        /* -1081 * a_{k-33}  if k >= 33 */
        if (k >= 33) {
            sum -= mul_small(cache[k - 33], 1081);
        }

        cache[k] = term_p + sum;
    }

    last_computed = n;
    return cache[n];
}
