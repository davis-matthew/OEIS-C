/*
 * A051109  Expansion of g.f. (1+2*x+5*x^2)/(1-10*x^3).
 * https://oeis.org/A051109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + (n mod 3)²) * 10^floor(n/3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;          /* floor(n/3)                 */
    unsigned r = (unsigned)(n % 3u);  /* n mod 3 (0,1,2)         */

    /* (1 + r²) is 1, 2, or 5 depending on r */
    seq_t mul = (seq_t)(1 + r * r);

    /* Compute 10^q by successive multiplication by 10 */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < q; ++i)
        pow10 *= 10;

    return mul * pow10;
}
