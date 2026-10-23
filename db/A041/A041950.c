/*
 * A041950  Numerators of continued fraction convergents to sqrt(498).
 * https://oeis.org/A041950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(498)
   Recurrence: a(0) = 22, a(1) = 67
   for n >= 2
       a(n) = c_n * a(n-1) + a(n-2)
   where the continued‑fraction coefficients are
       c_0 = 22
       c_n = (3, 6, 22, 6, 3, 44) repeated for n >= 1
*/
static const seq_t PERIOD[6] = { (seq_t)3, (seq_t)6, (seq_t)22,
                                 (seq_t)6, (seq_t)3, (seq_t)44 };

seq_t a(idx_t n)
{
    /* p_{-2} = 0, p_{-1} = 1 are the standard numerators’
       base values for convergents. */
    seq_t p_minus2 = (seq_t)0;
    seq_t p_minus1 = (seq_t)1;
    seq_t cur = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)22;            /* first coefficient a0 */
        } else {
            coeff = PERIOD[(i - 1) % 6];  /* periodic part */
        }
        cur = coeff * p_minus1 + p_minus2;  /* recurrence */
        p_minus2 = p_minus1;
        p_minus1 = cur;
    }
    return cur;
}
