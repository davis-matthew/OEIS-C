/*
 * A120582  Hankel transform of Sum_{k=0..floor(n/2)} binomial(2*k, k).
 * https://oeis.org/A120582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-2) - 16*a(n-4), a(0)=1, a(1)=2, a(2)=0, a(3)=0
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 33, i64 n <= 63, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implements the linear recurrence derived from the generating function
    (1-2*x)*(1+2*x)^2/(1 - 4*x^2 + 16*x^4).  The array a[4] holds the last
    four values so that a(n) can be computed in O(n) time and O(1) space. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0,1,2,3 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)0;

    seq_t a[4] = { (seq_t)1, (seq_t)2, (seq_t)0, (seq_t)0 }; /* a[0..3] */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using 4*a(i-2) - 16*a(i-4) */
        seq_t val = 4 * a[(i - 2) & 3] - 16 * a[(i - 4) & 3];
        a[i & 3] = val;
    }
    return a[n & 3];
}
