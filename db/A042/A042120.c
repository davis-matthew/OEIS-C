/*
 * A042120  Numerators of continued fraction convergents to sqrt(585).
 * https://oeis.org/A042120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = a(n-1) * c(n) + a(n-2) for n >= 1 with a(0) = 24, a(1) =
 *            121, where c(n) are the continued fraction coefficients of
 *            sqrt(585) (a(0)=24, then 5,2,1,4,1,2,5,48 repeating)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS sequence A042120 */
seq_t a(idx_t n)
{
    /* Continued fraction coefficients for sqrt(585) */
    static const seq_t period[8] = {5, 2, 1, 4, 1, 2, 5, 48};

    /* Initialise h_{-2} = 0, h_{-1} = 1 */
    seq_t h_minus2 = 0, h_minus1 = 1, h_current = 0;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = 24;
        else
            a_i = period[(i - 1) % 8];

        h_current = a_i * h_minus1 + h_minus2;
        h_minus2 = h_minus1;
        h_minus1 = h_current;
    }
    return h_current;
}
