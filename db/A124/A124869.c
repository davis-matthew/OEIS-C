/*
 * A124869  Numerator of real part of (3*i - 1)^(-n).
 * https://oeis.org/A124869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124869: numerator of the real part of (3*i-1)^(-n). 
   Let s_n = Re((-1-3i)^n).  Then 
       s_0 = 1,      s_1 = -1, 
       s_{n+2} = -2*s_{n+1} - 10*s_n   (n ≥ 0).
   The real part of (3*i-1)^(-n) equals s_n / 10^n, so the
   required numerator is 
       a(n) = s_n / gcd(s_n, 10^n).  */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -1;         /* s_1 */

    /* compute s_n via the second‑order recurrence */
    seq_t s_prevprev = 1;   /* s_0 */
    seq_t s_prev     = -1;  /* s_1 */
    seq_t s_curr;
    for (idx_t k = 2; k <= n; ++k) {
        s_curr = - (2 * s_prev + 10 * s_prevprev);
        s_prevprev = s_prev;
        s_prev     = s_curr;
    }
    seq_t s = s_curr;        /* s_n */

    /* exponent of 2 in s  (v2) */
    idx_t exp2 = 0;
    seq_t tmp = s;
    while (tmp != 0 && tmp % 2 == 0) {
        tmp /= 2;
        ++exp2;
    }

    /* exponent of 5 in s  (v5) */
    idx_t exp5 = 0;
    tmp = s;
    while (tmp != 0 && tmp % 5 == 0) {
        tmp /= 5;
        ++exp5;
    }

    /* limit to the powers actually dividing 10^n */
    if (exp2 > n) exp2 = n;
    if (exp5 > n) exp5 = n;

    /* cancel common factors 2^exp2 and 5^exp5 */
    for (idx_t i = 0; i < exp2; ++i) s /= 2;
    for (idx_t i = 0; i < exp5; ++i) s /= 5;

    return s;
}
