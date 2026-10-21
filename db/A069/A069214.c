/*
 * A069214  Let u(n,k) be the recursion defined by u(n,1)=1, u(n,2)=2,
 *          u(n,3)=n, and u(n,k+3) = (u(n,k+2) + u(n,k+1))/u(n,k) if u(n,k)
 *          divides u(n,k+2) + u(n,k+1), u(n,k+3) = u(n,k) otherwise. Then
 *          u(n,k) is periodic and a(n) = Max(u(n,k), k >= 1).
 * https://oeis.org/A069214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=5; for n≥2, if n is even a(n)=n+2, if n is odd
 *            a(n)=(3n+1)/2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)          /* Special case for n = 1 */
        return (seq_t)5;

    if ((n & 1u) == 0u)   /* n even  */
        return (seq_t)n + (seq_t)2;

    /* n odd and n ≥ 3 */
    /* Compute (3*n + 1) / 2 using seq_t arithmetic only */
    seq_t t = (seq_t)3 * (seq_t)n + (seq_t)1;
    return t >> 1;      /* division by 2 */
}
