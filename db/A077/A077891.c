/*
 * A077891  Expansion of (1-x)^(-1)/(1+2*x^2-2*x^3).
 * https://oeis.org/A077891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1-x)^(-1)/(1+2*x^2-2*x^3).
   The coefficients h_k of 1/(1+2x^2-2x^3) satisfy
     h_0 = 1,  h_1 = 0,  h_2 = -2,
     h_k = -2*h_{k-2} + 2*h_{k-3}  (k ≥ 3).
   The desired sequence a(n) is the cumulative sum:
     a(n) = Σ_{k=0..n} h_k.  */

/* Note: idx_t is unsigned (uint64_t), seq_t is signed (in
   the build).  All computations use seq_t. */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* Handle trivial case n = 0 quickly.  */
    if (n == 0)
        return (seq_t)1;

    /* Allocate array to hold h[0..n] – small for n up to 10⁴.  */
    size_t len = (size_t)n + 1;
    seq_t *h = (seq_t *)malloc(len * sizeof(seq_t));
    if (!h)  /* Allocation failure – return 0 as fallback. */
        return (seq_t)0;

    /* Compute h[0], h[1], h[2] (if needed) and the partial sums. */
    h[0] = (seq_t)1;
    if (len > 1)
        h[1] = (seq_t)0;
    if (len > 2)
        h[2] = (seq_t)-2;

    seq_t sum = h[0];
    if (len > 1)
        sum += h[1];
    if (len > 2)
        sum += h[2];

    for (size_t i = 3; i < len; ++i) {
        /* h[i] = -2*h[i-2] + 2*h[i-3]  */
        h[i] = (seq_t)(-2) * h[i - 2] + (seq_t)2 * h[i - 3];
        sum += h[i];
    }

    free(h);
    return sum;
}
