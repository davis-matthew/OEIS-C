/*
 * A042926  Numerators of continued fraction convergents to sqrt(995).
 * https://oeis.org/A042926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(995) (continued
 *            fraction expansion of sqrt(995) is [31;
 *            (1,1,5,4,3,12,3,4,5,1,1,62)] repeated)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned cycle[12] = {1, 1, 5, 4, 3, 12, 3, 4, 5, 1, 1, 62};

    seq_t p_minus2 = 0;    /* p_{-2} */
    seq_t p_minus1 = 1;    /* p_{-1} */
    seq_t p = 0;           /* current numerator */

    idx_t k;
    for (k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)31;
        } else {
            idx_t idx = (k - 1) % 12;
            a_k = (seq_t)cycle[idx];
        }
        p = a_k * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;
    }
    return p;
}
