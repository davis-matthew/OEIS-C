/*
 * A085463  Number of 5-tuples (v1,v2,v3,v4,v5) of nonnegative integers less
 *          than n such that v1 <= v4, v1 <= v5, v2 <= v4, v2 <= v5 and v3 <=
 *          v4.
 * https://oeis.org/A085463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 10*binomial(n,2) + 30*binomial(n,3) +
 *            35*binomial(n,4) + 14*binomial(n,5)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 129, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For safety we handle n == 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;            /* cast once to the result type */

    /* Compute binom(n,2) … binom(n,5) using multiplicative recurrence
       to keep intermediate values small. */
    seq_t c2 = nn;                  /* binom(n,1) */
    c2 *= (nn - 1);                 /* n*(n-1)   */
    c2 /= 2;                        /* /2        */

    seq_t c3 = c2;                  /* binom(n,2)*/
    c3 *= (nn - 2);                 /* * (n-2)   */
    c3 /= 3;                        /* /3        */

    seq_t c4 = c3;                  /* binom(n,3)*/
    c4 *= (nn - 3);                 /* * (n-3)   */
    c4 /= 4;                        /* /4        */

    seq_t c5 = c4;                  /* binom(n,4)*/
    c5 *= (nn - 4);                 /* * (n-4)   */
    c5 /= 5;                        /* /5        */

    /* Assemble the final result. */
    seq_t res = nn;                 /* n */
    res += (seq_t)10 * c2;          /* 10*binom(n,2) */
    res += (seq_t)30 * c3;          /* 30*binom(n,3) */
    res += (seq_t)35 * c4;          /* 35*binom(n,4) */
    res += (seq_t)14 * c5;          /* 14*binom(n,5) */

    return res;
}
