/*
 * A214917  a(n) is the least m > 0 such that Fibonacci(n-m) divides
 *          Fibonacci(n+1+m).
 * https://oeis.org/A214917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest m>0 such that Fibonacci(n-m) divides
 *            Fibonacci(n+1+m)
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the property that F_d divides F_k iff either d∈{1,2} or d|k. */

/* Assumes seq_t and idx_t are defined by the build (idx_t is unsigned 64-bit). */
#include <stdbool.h>  /* for bool if needed */

seq_t a(idx_t n)
{
    /* n >= 2 by caller's definition*/
    idx_t m;
    for (m = 1; m < n; ++m) {
        idx_t d = n - m;              /* n-m >= 1 */
        if (d <= 2)                   /* F_1, F_2 always divide everything */
            return (seq_t)m;
        idx_t t = n + 1 + m;          /* n+1+m */
        if (t % d == 0)
            return (seq_t)m;
    }
    /* Should never reach here for n>=2, but return 0 as a safe fallback. */
    return (seq_t)0;
}
