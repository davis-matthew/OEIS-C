/*
 * A008821  Expansion of (1+2*x^9+x^16)/((1-x^2)^2*(1-x^16)).
 * https://oeis.org/A008821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008821: expansion of (1+2*x^9+x^16)/((1-x^2)^2*(1-x^16)) */
#include <stdint.h>

/* Helper: coefficient of 1/((1-x^2)^2*(1-x^16)) at x^n */
static seq_t S(idx_t n)
{
    if ((n & 1) != 0)       /* odd n => coefficient 0 */
        return 0;

    idx_t t = n >> 1;       /* n = 2*t */
    idx_t m = t / 8;        /* floor(t/8) */

    seq_t term1 = (seq_t)(m + 1);     /* (m+1) */
    seq_t term2 = (seq_t)(t + 1);     /* (t+1) */
    seq_t term3 = (seq_t)(4 * m);     /* 4*m */

    return term1 * (term2 - term3);   /* (m+1)*(t+1-4m) */
}

seq_t a(idx_t n)
{
    seq_t res = S(n);
    if (n >= 9)   res += 2 * S(n - 9);
    if (n >= 16)  res += S(n - 16);
    return res;
}
