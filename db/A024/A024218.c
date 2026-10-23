/*
 * A024218  a(n) = s(1)*s(2)*...*s(n+1)(1/s(2) - 1/s(3) + ... + c/s(n+1)),
 *          where c=(-1)^n+1 and s(k) = 3k-2 for k = 1,2,3,...
 * https://oeis.org/A024218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A024218:  a(n) = Π_{k=1}^{n+1} s(k)  ·  Σ_{k=2}^{n+1} (-1)^k / s(k)
   where   s(k) = 3k-2  and  the series ends with the term (–1)^{n+1}/s(n+1).  */

/* The recurrence used is:
        a_{n+1} = a_n · s(n+2)  +  (-1)^{n+2} · P_n
    where  P_n = Π_{k=1}^{n+1} s(k).
   This allows an O(n) computation using only integer arithmetic.  */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence; values for n<1 are undefined. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_val = (seq_t)1;        /* a_1 */
    seq_t P     = (seq_t)1 * (seq_t)4;   /* P_1 = s(1)*s(2) = 1*4 */

    for (idx_t i = 1; i < n; ++i)   /* iterate from n=1 to n-1 to compute a_n */
    {
        /* s(i+2) = 3*(i+2)-2 = 3i + 4 */
        seq_t si2 = (seq_t)3 * (seq_t)(i + 2) - (seq_t)2;

        /* sign = (-1)^{i+2} : +1 for even exponent, -1 for odd exponent */
        int sign = (((i + 2) & 1U) == 0) ? 1 : -1;

        /* a_{i+1} = a_i * s(i+2) + sign * P */
        if (sign > 0)
            a_val = a_val * si2 + P;
        else
            a_val = a_val * si2 - P;

        /* update P for the next iteration: P_{i+1} = P_i * s(i+2) */
        P = P * si2;
    }

    return a_val;
}
