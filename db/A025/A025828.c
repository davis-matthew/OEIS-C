/*
 * A025828  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^6)).
 * https://oeis.org/A025828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor((n+3)/3) - floor((n+1)/4)) * * (floor((n+3)/3) +
 *            floor((n+1)/4) - floor((n+1)/2))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* integer divisions are exact for unsigned n */
    idx_t f1 = (n + 3) / 3;
    idx_t f2 = (n + 1) / 4;
    idx_t f3 = (n + 1) / 2;

    /* cast to seq_t to avoid overflow in the product */
    seq_t term1 = (seq_t)(f1 - f2);
    seq_t term2 = (seq_t)(f1 + f2 - f3);
    return term1 * term2;
}
