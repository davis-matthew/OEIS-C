/*
 * A139632  Expansion of chi(q) * chi(-q^5) in powers of q where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A139632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 279, u32 n <= 823, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A139632
 *  Expansion of χ(q) * χ(−q^5) in powers of q.
 *  G.f.: Product_{k>0} (1 + x^k) / ((1 + x^{2k}) * (1 + x^{5k}))
 *
 *  The coefficient a(n) is computed by multiplying the factors
 *  (1 + x^k)^{e_k} with e_k = 1 – [k even] – [k mod 5 = 0].
 *  For e_k = 1 we multiply by (1 + x^k); for e_k = −1 we multiply
 *  by the infinite series 1/(1 + x^k) = Σ_{m≥0} (-1)^m x^{m k}.
 *  All intermediate calculations use the seq_t type, which supports
 *  signed arithmetic, so cancellations are handled correctly.
 *
 *  The implementation reallocates the coefficient array only when a
 *  larger index is requested.  It uses the helper functions
 *  seq_alloc_new_array and seq_free from the utils package to
 *  allocate and free the arrays so that the code compiles both in
 *  C11 and C++17 where seq_t may be a class type.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

/* static cache for computed coefficients */
static seq_t *coeff      = NULL;
static idx_t  max_n      = (idx_t)-1;

/* temporary buffer used when multiplying by 1/(1 + x^k) */
static seq_t *tmp_buf    = NULL;
static idx_t  tmp_len    = 0;

/* ensure tmp_buf has room for at least n+1 coefficients */
static void
ensure_tmp_buf(idx_t n)
{
    if (tmp_len < n + 1) {
        seq_t *old = tmp_buf;
        tmp_buf = seq_alloc_new_array((size_t)(n + 1));
        if (old != NULL)
            seq_free(old);
        tmp_len = n + 1;
    }
}

/* recompute coefficients up to n (inclusive) from scratch */
static void
recompute(idx_t n)
{
    /* allocate new coefficient array */
    seq_t *new_coeff = seq_alloc_new_array((size_t)(n + 1));
    if (coeff != NULL)
        seq_free(coeff);
    coeff = new_coeff;
    max_n = n;

    /* initialize polynomial */
    coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        coeff[i] = (seq_t)0;

    /* ensure temporary buffer */
    ensure_tmp_buf(n);

    /* sequentially apply factors k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        int e = 1;
        if ((k & 1) == 0)    /* k even   */
            --e;
        if (k % 5 == 0)       /* k multiple of 5 */
            --e;

        if (e == 1) {                      /* multiply by (1 + x^k) */
            for (idx_t i = n; i >= k; --i)
                coeff[i] = coeff[i] + coeff[i - k];
        }
        else if (e == -1) {                /* multiply by 1/(1 + x^k) */
            /* copy current polynomial to temporary buffer */
            for (idx_t i = 0; i <= n; ++i)
                tmp_buf[i] = coeff[i];

            /* apply the series expansion Σ (-1)^m x^{m k} */
            idx_t max_m = n / k;
            seq_t one = (seq_t)1;
            for (idx_t m = 1; m <= max_m; ++m) {
                seq_t sign = ((m & 1) == 0) ? one : -one;
                idx_t offset = m * k;
                for (idx_t i = offset; i <= n; ++i)
                    tmp_buf[i] = tmp_buf[i] + sign * coeff[i - offset];
            }

            /* copy back the updated coefficients */
            for (idx_t i = 0; i <= n; ++i)
                coeff[i] = tmp_buf[i];
        }
        /* e == 0 : factor is 1, do nothing */
    }
}

/* public routine requested by the evaluation harness */
seq_t a(idx_t n)
{
    if (coeff == NULL || max_n < n)
        recompute(n);
    return coeff[n];
}
