/*
 * A076736  Let u(1) = u(2) = u(3) = 2, u(n) = (1 + u(n-1)*u(n-2))/u(n-3);
 *          then a(n) is the denominator of u(n).
 * https://oeis.org/A076736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A076736 – denominator of u(n) defined by u(1)=u(2)=u(3)=2,
 * u(n) = (1 + u(n-1)*u(n-2))/u(n-3).
 * For n > 5, a(n) = 2 * a(n-2); a(1)=1, a(2)=1, a(3)=1,
 * a(4)=2, a(5)=1.
 * (Uses the recurrence; no need for memoization.)
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    static const seq_t init[5] = {1, 1, 1, 2, 1};
    if (n <= 5)
        return init[n - 1];

    /* For n > 5 use the recurrence a(n)=2*a(n-2). */
    /* Exponent of 2 is: */
/*  even n: exp = n/2 - 1
 *  odd  n: exp = n/2 - 2    (integer division) */
    idx_t exp = (n & 1) ? (n / 2 - 2) : (n / 2 - 1);
    return pow2(exp);
}
