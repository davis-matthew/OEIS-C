/*
 * A073358  Binomial transform, alternating in sign, of the tribonacci
 *          numbers.
 * https://oeis.org/A073358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3), a(0) = 0, a(1) = -1, a(2) = -1
 * Range:     i8 n <= 14, i16 n <= 25, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A073358.  The calculation is done
   iteratively, only using O(1) additional space, and works for any
   idx_t/seq_t combination that satisfies the type constraints. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1 || n == 2)
        return (seq_t)-1;

    /* Iteratively compute the terms from 3 up to n.
       Only the last three values are needed. */
    seq_t a0 = (seq_t)0;    /* a(n-3) */
    seq_t a1 = (seq_t)-1;   /* a(n-2) */
    seq_t a2 = (seq_t)-1;   /* a(n-1) */
    seq_t anum;             /* current term a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        anum = (seq_t)2 * a2 - (seq_t)2 * a0;  /* a(i) = 2*a(i-1) - 2*a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = anum;
    }
    return a2;
}
