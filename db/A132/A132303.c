/*
 * A132303  Sum of cubes of trinomial coefficients: a(n) = Sum_{k=0..2n}
 *          trinomial(n,k)^3 where trinomial(n,k) = [x^k] (1 + x + x^2)^n.
 * https://oeis.org/A132303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2*n} trinomial(n,k)^3 where trinomial(n,k) is
 *            the coefficient of x^k in (1 + x + x^2)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

/* Computes a(n) for any n ≥ 0.  The function builds the
   trinomial triangle up to row n with the recurrence
   c(n,k) = c(n‑1,k) + c(n‑1,k‑1) + c(n‑1,k‑2),
   then sums the cubes of the 2n + 1 coefficients.    */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t sz = (size_t)(2 * n + 1);
    seq_t *prev = alloc_seq_array(sz);
    seq_t *cur  = alloc_seq_array(sz);
    if (!prev || !cur) {
        free_seq(prev);
        free_seq(cur);
        return (seq_t)0;
    }

    /* prev is zero‑initialised by alloc_seq_array.  Set base. */
    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t maxk = 2 * i;
        for (idx_t j = 0; j <= maxk; ++j) {
            seq_t val = prev[j];
            if (j > 0)   val += prev[j-1];
            if (j > 1)   val += prev[j-2];
            cur[j] = val;
        }
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t t   = prev[k];
        seq_t t2  = t * t;
        seq_t cube = t2 * t;
        sum += cube * (seq_t)2;   /* account for symmetry */
    }
    seq_t mid   = prev[n];
    seq_t mid2  = mid * mid;
    seq_t mid3  = mid2 * mid;
    sum += mid3;      /* middle term */

    free_seq(prev);
    free_seq(cur);
    return sum;
}
