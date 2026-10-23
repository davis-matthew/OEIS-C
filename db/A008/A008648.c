/*
 * A008648  Molien series of 3 X 3 upper triangular matrices over GF( 5 ).
 * https://oeis.org/A008648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n+15)*(n+20) - 2*(n mod 5)*(n+10))/250)
 * Range:     u8 none, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t mod5 = (seq_t)(n % 5);
    seq_t num = (nn + 15) * (nn + 20) - 2 * mod5 * (nn + 10);
    return num / (seq_t)250;
}
