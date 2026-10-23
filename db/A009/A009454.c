/*
 * A009454  Expansion of e.g.f. sin(log(1+x)).
 * https://oeis.org/A009454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+2) = -(2n+1)*a(n+1) - (n^2+1)*a(n), with a(0)=0, a(1)=1
 *            (OEIS A009454)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function loops over the recurrence, using only integer arithmetic. */

#include <stddef.h> /* for size_t, if needed                       */

/* ----------------------------------------------------------
 * Public function: sequence A009454
 * ---------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* recurrence variables */
    seq_t prev2 = (seq_t)0; /* a(n-2) */
    seq_t prev1 = (seq_t)1; /* a(n-1) */
    seq_t curr;

    /* iterate from 2 up to n */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t m = k - 2;                      /* current n in the recurrence */
        seq_t mseq = (seq_t)m;

        /* compute coefficients using only integer ops */
        seq_t coeff1 = -( (seq_t)2 * mseq + (seq_t)1 );          /* -(2n+1) */
        seq_t coeff2 = -( mseq * mseq + (seq_t)1 );             /* -(n^2+1) */

        curr = coeff1 * prev1 + coeff2 * prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1; /* holds a(n) after loop completion */
}
