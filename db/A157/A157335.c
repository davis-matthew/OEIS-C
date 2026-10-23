/*
 * A157335  Expansion of 1/( (1+x)*(1-7*x+x^2) ).
 * https://oeis.org/A157335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - a(n-2) + (-1)^n (a(0)=1, a(1)=6)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A157335_H
#define A157335_H

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)1;       /* a(0) */
    seq_t prev1 = (seq_t)6;       /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)7;   /* 7*a(i-1)   */
        curr -= prev2;             /* 7*a(i-1)-a(i-2) */

        /* add the constant term (-1)^i */
        if ((i & (idx_t)1) == 0)        /* i even: (-1)^i = +1 */
            curr += (seq_t)1;
        else                            /* i odd : (-1)^i = -1 */
            curr -= (seq_t)1;

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;   /* a(n) */
}

#endif /* A157335_H */
