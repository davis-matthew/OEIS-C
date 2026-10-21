/*
 * A088837  Numerator of sigma(2*n)/sigma(n). Denominator see in A038712.
 * https://oeis.org/A088837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = (2*n) XOR (2*n-1) (numerator of sigma(2*n)/sigma(n))
 * Range:     u8 n <= 127, u16 n <= 8714, u32 n <= 8714, u64 n <= 8714,
 *            u128 n <= 8714, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t first so all arithmetic uses seq_t types                     */
    seq_t two_n = (seq_t)n * 2;          /* compute 2*n                               */
    return two_n ^ (two_n - 1);          /* XOR with (2*n-1) to get the term          */
}
