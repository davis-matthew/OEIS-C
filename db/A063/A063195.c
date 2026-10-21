/*
 * A063195  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          6 ).
 * https://oeis.org/A063195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; for n≥2 let k=n/6 (integer division) and r=n%6. Then
 *            a(n)= 2*k-1 if r==1, otherwise a(n)=2*k+1
 * Range:     u8 n <= 767, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    idx_t k = n / 6;
    unsigned r = n % 6;
    if (r == 1)
        return (seq_t)(2 * k - 1);
    else
        return (seq_t)(2 * k + 1);
}
