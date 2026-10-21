/*
 * A080750  a(n) = largest number greater than a(n-1) such that the first n
 *          terms of the sequence contain a total of a(n) base-10 digits.
 * https://oeis.org/A080750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = largest x > a(n-1) such that x = sum_{i=1}^n digits(a(i))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   compute_val, digit_len_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (digits(a(i)) = number of base‑10 digits of a(i))   */

#include <stdlib.h>
#include "../utils/compute_val.h"
#include "../utils/digit_len_local.h"

seq_t a(idx_t n)
{
    static seq_t *tbl = NULL;     /* table of a(n), indexed by 0 … */
    static size_t tbl_cap = 0;    /* current capacity */
    static size_t tbl_len = 0;    /* number of values stored */
    static seq_t *cum = NULL;     /* cumulative digit sums, indexed by 1 … */
    static size_t cum_cap = 0;
    static size_t cum_len = 0;

    if (n == 0)
        return 0;   /* arbitrary, never used by the OEIS offset rules */

    if (n == 1) {               /* base case */
        /* ensure at least one element allocated */
        if (tbl_len == 0) {
            tbl = (seq_t *)malloc(sizeof(seq_t) * 1);
            cum = (seq_t *)malloc(sizeof(seq_t) * 1);
            tbl_cap = cum_cap = 1;
            tbl_len = cum_len = 1;
            tbl[0] = (seq_t)1;
            cum[0] = (seq_t)1;  /* sum of digits of first term */
        }
        return tbl[0];
    }

    /* grow tables if necessary */
    if ((size_t)n > tbl_cap) {
        size_t new_cap = tbl_cap ? tbl_cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = n;
        tbl = (seq_t *)realloc(tbl, sizeof(seq_t) * new_cap);
        cum = (seq_t *)realloc(cum, sizeof(seq_t) * new_cap);
        tbl_cap = cum_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = tbl_len; i < (size_t)n; ++i) {
        /* cumulative digit count up to previous term i */
        seq_t T = cum[i - 1];                 /* sum of digits of first i terms */
        seq_t val = compute_val(T);           /* a(i+1) */
        tbl[i] = val;
        int dl = digit_len_local(val);
        cum[i] = T + (seq_t)dl;
    }
    tbl_len = cum_len = n;

    return tbl[n - 1];
}
