/*
 * A085505  a(n) = ceiling(((1*n^0 + 1*n^1 + 2*n^2 + 4*n^3)/(1*n^0 + 2*n^1 +
 *          1*n^2))^2).
 * https://oeis.org/A085505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(((1*n^0 + 1*n^1 + 2*n^2 + 4*n^3)/(1*n^0 + 2*n^1 +
 *            1*n^2))^2)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* work in 128‑bit to avoid overflow when squaring */
    unsigned __int128 nn = (unsigned __int128)(seq_t)n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;

    unsigned __int128 N = 1 + nn + 2 * n2 + 4 * n3;   /* numerator of ratio */
    unsigned __int128 D = 1 + 2 * nn + n2;           /* denominator of ratio */

    unsigned __int128 num = N * N;   /* (N/D)^2 numerator */
    unsigned __int128 den = D * D;   /* (N/D)^2 denominator */

    /* ceil(num/den) = (num + den - 1)/den */
    seq_t result = (seq_t)((num + den - 1) / den);
    return result;
}
