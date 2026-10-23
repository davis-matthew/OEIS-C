/*
 * A106251  Expansion of (1-x+x^2+x^3+x^5)/(1-x-x^6+x^7).
 * https://oeis.org/A106251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/6⌋} (k+1) * c[n-6k] with c =
 *            {1,0,1,2,2,3,2,3,2,1,1} (coefficients of
 *            (1+x²+2x³+2x⁴+3x⁵+2x⁶+3x⁷+2x⁸+x⁹+x¹⁰) )
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char coeff[11] = {
    1, 0, 1, 2, 2, 3,
    2, 3, 2, 1, 1
};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 6;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = n - 6 * k;
        if (r <= 10u)
            sum += (seq_t)(k + 1u) * coeff[r];
    }
    return sum;
}
