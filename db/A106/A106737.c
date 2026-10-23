/*
 * A106737  a(n) = Sum_{k=0..n} ({binomial(n+k,n-k)*binomial(n,k)} mod 2).
 * https://oeis.org/A106737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k, n-k)*binomial(n,k) mod 2
 *            Recurrence: a(0)=1, a(2n)=a(n), a(4n+1)=2*a(2n),
 *            a(4n+3)=2*a(2n+1)-a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if ((n & 1) == 0) return a(n >> 1);
    if ((n & 3) == 1) return (seq_t)2 * a(n >> 1);
    return (seq_t)2 * a(n >> 1) - a((n - 3) >> 2);
}
