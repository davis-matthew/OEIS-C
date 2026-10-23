/*
 * A135353  a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3), with a(0) = 2, a(1) = -3,
 *          a(2) = 3.
 * https://oeis.org/A135353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3), * a(0) = 2, a(1) = -3,
 *            a(2) = 3
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of sequence A135353.  The function is
 * self‑contained: it only uses the built‑in arithmetic operators
 * for the type seq_t and the index type idx_t.  The sequence is
 * defined by the linear recurrence presented in the comment.
 */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)  2;
    if (n == 1) return (seq_t) -3;
    if (n == 2) return (seq_t)  3;

    /* iterative evaluation of the recurrence */
    seq_t a0 = (seq_t)  2;   /* a(n‑3) */
    seq_t a1 = (seq_t) -3;   /* a(n‑2) */
    seq_t a2 = (seq_t)  3;   /* a(n‑1) */

    /* number of steps needed: from a(3) up to a(n) inclusive */
    idx_t steps = n - 2;     /* n ≥ 3 by the early return */
    idx_t s;

    for (s = 1; s <= steps; ++s) {
        seq_t an = 3 * a2 - 3 * a1 + 2 * a0;  /* a(n)  */
        a0 = a1;   /* shift windows forward */
        a1 = a2;
        a2 = an;
    }
    return a2;
}
