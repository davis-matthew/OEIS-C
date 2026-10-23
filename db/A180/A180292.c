/*
 * A180292  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-2.
 * https://oeis.org/A180292
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*binomial(n,n-2) for n>4, a(3)=1, a(4)=18
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the exceptional small cases */
    if (n == 3)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)18;

    /* For n >= 5: a(n) = n * C(n,2) = n * (n*(n-1)/2) */
    seq_t sn   = (seq_t)n;               /* cast to seq_t */
    seq_t t    = sn * (sn - (seq_t)1);   /* n*(n-1) */
    t = t / (seq_t)2;                    /* divide by 2 */
    return sn * t;                       /* multiply by n again */
}
