/*
 * A233581  a(n) = 2*a(n-1) - 3*a(n-2) + a(n-3), a(0) = 1, a(1) = 0, a(2) =
 *          -1.
 * https://oeis.org/A233581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + a(n-3), a(0)=1, a(1)=0, a(2)=-1
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 53, i64 n <= 105,
 *            i128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-1;

    /* Use a sliding window of the last three computed terms. */
    seq_t a0 = (seq_t)1;   /* a(k-3) */
    seq_t a1 = (seq_t)0;   /* a(k-2) */
    seq_t a2 = (seq_t)-1;  /* a(k-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)2 * a2 - (seq_t)3 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
