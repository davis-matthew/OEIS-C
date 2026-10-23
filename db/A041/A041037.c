/*
 * A041037  Denominators of continued fraction convergents to sqrt(23).
 * https://oeis.org/A041037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 1, a(1) = 1, for n≥2, a(n) = coeff(n) * a(n-1) + a(n-2),
 *            where coeff(0) = 4 and for n≥1 coeff(n) = 1, 3, 1, 8 (period 4)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* previous two terms: a(-1) = 0, a(-2) = 1 */
    seq_t prev2 = 1;   /* a(-2) */
    seq_t prev1 = 0;   /* a(-1) */
    seq_t curr = 0;    /* a(n) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coef;

        if (i == 0) {
            coef = 4;
        } else {
            /* pattern 1,3,1,8 repeats with period 4 */
            switch ((i - 1) & 3u) {
                case 0: coef = 1; break;
                case 1: coef = 3; break;
                case 2: coef = 1; break;
                case 3: coef = 8; break;
            }
        }

        curr = coef * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
