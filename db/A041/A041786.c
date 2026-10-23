/*
 * A041786  Numerators of continued fraction convergents to sqrt(414).
 * https://oeis.org/A041786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(414)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int period[8] = {2, 1, 7, 2, 7, 1, 2, 40};

    seq_t p_minus1 = 1; /* p_{-1} */
    seq_t p_minus2 = 0; /* p_{-2} */
    seq_t p;

    for (idx_t i = 0; i <= n; ++i) {
        int coeff = (i == 0) ? 20 : period[(i - 1) % 8];
        p = ((seq_t)coeff * p_minus1) + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;
    }
    return p;
}
