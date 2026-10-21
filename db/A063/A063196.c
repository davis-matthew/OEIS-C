/*
 * A063196  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          7 ).
 * https://oeis.org/A063196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n=1; for n>1, a(n) = n if n is odd, otherwise
 *            a(n)=n-1. This follows from the recurrence
 *            a(n)=a(n-1)+a(n-2)-a(n-3) for n>4 with initial values
 *            a(1)=0,a(2)=1,a(3)=3,a(4)=3
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    /* n>=2 */
    return (n & 1) ? (seq_t)n : (seq_t)(n - 1);
}
