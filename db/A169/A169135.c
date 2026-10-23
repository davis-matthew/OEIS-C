/*
 * A169135  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 26; For n >= 2, a(n) = 26*a(n-1) -
 *            24*sum_{i=0}^{n-2} a(i) - 2 Rewrite with prefix sums
 *            S(n)=sum_{i=0}^n a(i): S(0)=1, S(1)=27, For n >= 2, S(n) =
 *            27*S(n-1) - 50*S(n-2) - 2 and a(n) = S(n) - S(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;

    /* Prefix sums: S0 = S(0), S1 = S(1) */
    seq_t S_prevprev = (seq_t)1;   /* S(0) */
    seq_t S_prev     = (seq_t)27;  /* S(1) */

    const seq_t twenty_seven = (seq_t)27;
    const seq_t fifty        = (seq_t)50;
    const seq_t two          = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t S_curr = twenty_seven * S_prev - fifty * S_prevprev - two;
        if (i == n)            /* S_curr = S(n)  and S_prev = S(n-1) */
            return S_curr - S_prev;
        S_prevprev = S_prev;
        S_prev     = S_curr;
    }
    /* Should never reach here */
    return (seq_t)0;
}
