/*
 * A170348  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1 let d_i = 1 if i even, -2 if i odd. For n≤42:
 *            a[n] = 1 - Σ_{i=1}^{n} d_i·a[n-i] For n>42: a[n] = -
 *            Σ_{i=1}^{42} d_i·a[n-i]
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* NULL, free */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) → seq_t* */
#include "../utils/seq_free_array.h"    /* seq_free_array(seq_t*) */

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)1;

    /* allocate storage for all terms up to n */
    seq_t *A = alloc_seq_array((size_t)(n + 1));
    if (A == NULL) return (seq_t)0;   /* allocation failed – sentinel */

    A[0] = (seq_t)1;

    /* iterate computing a[1] … a[n] */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum_even = (seq_t)0;   /* sum over even indices (d_i = +1) */
        seq_t sum_odd  = (seq_t)0;   /* sum over odd  indices (d_i = -2) */

        /* k runs from 1 to min(42,i) */
        idx_t kmax = (i < 42) ? i : 42;
        for (idx_t k = 1; k <= kmax; ++k) {
            seq_t term = A[i - k];
            if ((k & 1U) == 0U)   /* even k */
                sum_even += term;
            else                  /* odd k */
                sum_odd  += term;
        }

        seq_t temp;
        if (i <= 42) {
            /* a[i] = 1 - sum_even + 2*sum_odd */
            temp = sum_odd * 2U;   /* 2*sum_odd  (no overflow for needed n) */
            temp -= sum_even;      /* safe, see note in analysis */
            temp += (seq_t)1;
        } else {
            /* a[i] = -sum_even + 2*sum_odd  (i > 42) */
            temp = sum_odd * 2U;
            temp -= sum_even;
        }

        A[i] = temp;
    }

    seq_t result = A[n];
    seq_free_array(A);
    return result;
}
