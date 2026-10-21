/*
 * A059778  Expansion of 1 / product((1+q^(2*n+3))/(1-q^(2*n+2)), n=0..inf).
 * https://oeis.org/A059778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (t(n) - t(n-1)), where t(n)=1 if n is a
 *            triangular number (i.e., 8n+1 is a perfect square), otherwise
 *            t(n)=0
 * Range:     i8 n <= 7, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Helper: characteristic function of triangular numbers.
   Returns 1 if n is triangular, 0 otherwise. */
static seq_t is_triangular(idx_t n)
{
    seq_t y = (seq_t)8 * (seq_t)n + (seq_t)1;
    seq_t s = floor_sqrt(y);
    return (s * s == y) ? (seq_t)1 : (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t tn    = is_triangular(n);
    seq_t tprev = is_triangular(n - 1);
    seq_t diff  = tn - tprev;          /* -1, 0, or 1 */
    seq_t sign  = (n & 1) ? (seq_t)-1 : (seq_t)1;
    return sign * diff;
}
