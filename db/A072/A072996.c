/*
 * A072996  Coefficient of the highest power of q in the expansion of nu(0)=1,
 *          nu(1)=b and for n>=2, nu(n)=b*nu(n-1)+lambda*(n-1)_q*nu(n-2) with
 *          (b,lambda)=(2,1), where (n)_q=(1+q+...+q^(n-1)) and q is a root of
 *          unity.
 * https://oeis.org/A072996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1; for n ≥ 2, a(n) = 5 if n is even, otherwise
 *            a(n) = 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    return (n & (idx_t)1) ? (seq_t)2 : (seq_t)5;
}
