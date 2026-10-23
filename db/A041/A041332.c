/*
 * A041332  Numerators of continued fraction convergents to sqrt(180).
 * https://oeis.org/A041332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(180) * Continued
 *            fraction: [13; 2,2,2,26] repeating. * Initial values: a(0) = 13,
 *            a(1) = 27. * Recurrence: a(n) = c(n) * a(n-1) + a(n-2), * where
 *            c(0)=13, for n>0: * c(n)=26 if n≡4 (mod 4), otherwise c(n)=2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t p_i = 0;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)13;
        } else {
            /* For indices >=1: indices 1,2,3 -> coeff 2; index 4 -> 26, pattern repeats. */
            if (((i - 1) % 4) == 3) {
                coeff = (seq_t)26;
            } else {
                coeff = (seq_t)2;
            }
        }
        p_i = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_i;
    }
    return p_i;
}
