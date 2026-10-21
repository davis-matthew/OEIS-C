/*
 * A229818  Even bisection gives sequence a itself, n->a(2*(3*n+k)-1) gives
 *          k-th differences of a for k=1..3 with a(n)=n for n<2.
 * https://oeis.org/A229818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, hear, eigen
 * Formula:   not stated
 * Range:     i8 n <= 2074, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 0, a(1) = 1
 *   a(2*n)   = a(n)
 *   a(6*n+1) = a(n+1) - a(n)
 *   a(6*n+3) = a(n+2) - 2*a(n+1) + a(n)
 *   a(6*n+5) = a(n+3) - 3*a(n+2) + 3*a(n+1) - a(n)
 * This implementation evaluates the recurrence recursively,
 * removing trailing zeros (factor 2) first to avoid extra depth.
 * The recursion depth is O(log n), so the routine is well under
 * 2 s for n up to about 10 000. */
 
/* Include stdint.h only if seq_t/idx_t are not already defined. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases for n < 2 */
    if (n < 2)
        return (seq_t)n;

    /* Strip factors of 2: a(2*n) = a(n) */
    while ((n & 1) == 0)
        n >>= 1;

    /* Now n is odd; decompose modulo 6 */
    idx_t m = n / 6;
    idx_t r = n - 6 * m;   /* n % 6 */

    if (r == 1) {
        /* a(6*m+1) = a(m+1) - a(m) */
        return a(m + 1) - a(m);
    } else if (r == 3) {
        /* a(6*m+3) = a(m+2) - 2*a(m+1) + a(m) */
        seq_t t0 = a(m);
        seq_t t1 = a(m + 1);
        seq_t t2 = a(m + 2);
        return t2 - 2 * t1 + t0;
    } else { /* r == 5 */
        /* a(6*m+5) = a(m+3) - 3*a(m+2) + 3*a(m+1) - a(m) */
        seq_t t0 = a(m);
        seq_t t1 = a(m + 1);
        seq_t t2 = a(m + 2);
        seq_t t3 = a(m + 3);
        return t3 - 3 * t2 + 3 * t1 - t0;
    }
}
