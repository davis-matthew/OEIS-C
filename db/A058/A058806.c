/*
 * A058806  a(n) = n! * H_n(n) where H_0(n) = 1/n, H_m(n) = Sum_{k=1..n}
 *          H_{m-1}(k).
 * https://oeis.org/A058806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(2*n-1)*a(n-1) - (2*n-3)!/(n-1)! (a(1) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes OEIS sequence A058806.
   The routine builds the values on demand and caches all values
   up to the largest n requested so far.  It works for any seq_t
   (plain integer or arbitrary‑precision class).  No dynamic
   allocation is used; a fixed array of size 10001 is sufficient
   for any call with 1 ≤ n ≤ 10000. */
seq_t a(idx_t n)
{
    /* The largest n we support is 10000 (given by the problem statement). */
    enum { MAX_N = 10001 };              /* array size = MAX_N + 1 */
    static seq_t A[MAX_N + 1];
    static seq_t R[MAX_N + 1];
    static idx_t cached = 0;              /* highest index computed so far */

    /* Initialise only once  */
    if (cached == 0) {
        A[1] = (seq_t)1;
        cached = 1;
    }

    if (n <= cached)                /* value already known */
        return A[n];

    /* Iteratively compute values up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t t;
        if (i == 2) {               /* (2*2-3)!/(2-1)! = 1 */
            t = (seq_t)1;
        } else {
            t = R[i - 1];
            t = t * (seq_t)(2 * i - 4);   /* multiply by (2*i-4) */
            t = t * (seq_t)(2 * i - 3);   /* multiply by (2*i-3) */
            t = t / (seq_t)(i - 1);       /* divide by (i-1) */
        }
        R[i] = t;

        seq_t coef = (seq_t)2 * (seq_t)(2 * i - 1);
        seq_t val  = A[i - 1] * coef;
        A[i] = val - t;
    }

    cached = n;
    return A[n];
}
