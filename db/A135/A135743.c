/*
 * A135743  E.g.f.: A(x) = Sum_{n>=0} exp(n*(n+1)/2*x)*x^n/n!.
 * https://oeis.org/A135743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A135743
 *  Formula: a(n) = Sum_{k=0..n} C(n,k) * [k*(k+1)/2]^(n-k)
 *  (Based on the given combinatorial sum)
 *
 *  Computes the value by incremental dynamic programming.
 *  Uses 3 arrays kept static across calls:
 *      a[n]      - the sequence values
 *      binom[k]  - current row of binomial coefficients of C(current_n, k)
 *      pow[k]    - T_k^(current_n-k)   where T_k = k*(k+1)/2
 *
 *  The arrays are resized with seq_alloc_new_array / seq_free_array
 *  to handle arbitrary‑precision seq_t types.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    /* static cache */
    static idx_t max_n = 0;          /* last n for which values are stored      */
    static seq_t *a_vals = NULL;     /* sequence values                         */
    static seq_t *binom = NULL;      /* current binomial row                    */
    static seq_t *pow_vals = NULL;   /* T_k^(current_n-k)                       */
    static size_t cache_len = 0;     /* allocated length of the arrays          */

    /* first call: initialise arrays for n=0 */
    if (cache_len == 0) {
        cache_len = 1;                      /* length = 0 + 1 */
        a_vals   = seq_alloc_new_array(cache_len);
        binom    = seq_alloc_new_array(cache_len);
        pow_vals = seq_alloc_new_array(cache_len);

        /* base case a(0) = 1, binom[0] = 1, pow[0] = 1 (T_0^0 = 1) */
        a_vals[0]   = 1;
        binom[0]    = 1;
        pow_vals[0] = 1;
        max_n = 0;
    }

    /* extend cache if needed */
    if (n > max_n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_a   = seq_alloc_new_array(new_len);
        seq_t *new_b   = seq_alloc_new_array(new_len);
        seq_t *new_p   = seq_alloc_new_array(new_len);

        /* copy old values */
        for (idx_t i = 0; i <= max_n; ++i) {
            new_a[i]   = a_vals[i];
            new_b[i]   = binom[i];
            new_p[i]   = pow_vals[i];
        }

        /* free old arrays */
        seq_free_array(a_vals);
        seq_free_array(binom);
        seq_free_array(pow_vals);

        a_vals   = new_a;
        binom    = new_b;
        pow_vals = new_p;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    for (idx_t curr = max_n + 1; curr <= n; ++curr) {

        /* update binomial row: Pascal's triangle */
        for (idx_t k = curr; k >= 1; --k) {
            binom[k] = binom[k] + binom[k - 1];
        }
        binom[0] = 1;   /* C(curr,0) = 1 */

        /* update powers: T_k^(curr-k) = previous * T_k (for k < curr) */
        for (idx_t k = 0; k < curr; ++k) {
            seq_t Tk = (seq_t)k * ((seq_t)k + 1) / 2;   /* triangular number */
            pow_vals[k] = pow_vals[k] * Tk;
        }
        pow_vals[curr] = 1;   /* T_curr^0 = 1 */

        /* sum over k to obtain a(curr) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= curr; ++k) {
            sum = sum + binom[k] * pow_vals[k];
        }
        a_vals[curr] = sum;
    }

    max_n = n;
    return a_vals[n];
}
