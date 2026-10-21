/*
 * A090294  a(n) = K_3(n) = Sum_{k>=0} A090285(3,k)*2^k*binomial(n,k). a(n) =
 *          (4*n^3+30*n^2+56*n+15)/3.
 * https://oeis.org/A090294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n^3+30*n^2+56*n+15)/3
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t val = 4 * n3 + 30 * n2 + 56 * N + 15;
    return val / 3;
}
