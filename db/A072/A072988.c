/*
 * A072988  Coefficient of the highest power of q in the expansion of nu(0)=1,
 *          nu(1)=b and for n>=2, nu(n)=b*nu(n-1)+lambda*(n-1)_q*nu(n-2) with
 *          (b,lambda)=(3,1), where (n)_q=(1+q+...+q^(n-1)) and q is a root of
 *          unity.
 * https://oeis.org/A072988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 10 if n ≥ 2 is even, otherwise 3
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    /* For n >= 2: even n → 10, odd n → 3 */
    return (n & 1ULL) ? (seq_t)3 : (seq_t)10;
}
