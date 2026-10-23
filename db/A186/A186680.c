/*
 * A186680  Total number of positive integers below 10^n requiring 17 positive
 *          biquadrates in their representation as sum of biquadrates.
 * https://oeis.org/A186680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=2}^{min(5,n)} c_k, where c = (0,0,3,30,30,2) are the
 *            coefficients of the numerator polynomial 3x²+30x³+30x⁴+2x⁵.
 *            Hence a(1)=0, a(2)=3, a(3)=33, a(4)=63, a(n)=65 for n≥5
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* early return for the offset */
    if (n < 2)
        return 0;

    /* coefficients of 3x² + 30x³ + 30x⁴ + 2x⁵ */
    static const seq_t coef[6] = {0, 0, 3, 30, 30, 2};

    seq_t res = 0;
    idx_t upper = (n < 5 ? n : 5);      /* max index to sum over */
    for (idx_t k = 2; k <= upper; ++k)
        res += coef[k];

    return res;
}
