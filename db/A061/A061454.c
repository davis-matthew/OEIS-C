/*
 * A061454  a(n) = denominator of the continued fraction [1; 2^2, 3^3, ...,
 *          n^n].
 * https://oeis.org/A061454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = n^n * a(n-1) + a(n-2), a(1) = 1, a(2) = 4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the denominator of the continued fraction
 * [1; 2^2, 3^3, …, n^n].
 *
 * The recurrence does not depend on any external data,
 * so the function works for any n ≥ 1.  It uses a
 * straightforward iterative implementation that
 * recomputes the needed power at every step.  The
 * algorithm runs in O(n log n) time and uses only
 * constant extra space.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* a(i-2)  : value for i-2
     * a(i-1)  : value for i-1
     * a(i)    : value for i
     */
    seq_t a_prevprev = (seq_t)1; /* a(1) */
    seq_t a_prev     = (seq_t)4; /* a(2) */
    seq_t a_cur      = a_prev;   /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute i^i using exponentiation by squaring */
        seq_t power = (seq_t)1;
        seq_t base  = (seq_t)i;
        idx_t exp   = i;
        while (exp > 0) {
            if (exp & 1U) power *= base;
            base *= base;
            exp >>= 1U;
        }
        /* recurrence */
        a_cur = power * a_prev + a_prevprev;
        /* slide the window */
        a_prevprev = a_prev;
        a_prev     = a_cur;
    }
    return a_cur;
}
