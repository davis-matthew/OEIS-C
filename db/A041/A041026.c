/*
 * A041026  Numerators of continued fraction convergents to sqrt(18).
 * https://oeis.org/A041026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=4, a(1)=17; for n≥2: a(n)=c_n*a(n-1)+a(n-2), where c_n = 4
 *            if n is odd, 8 if n is even
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev2 = 0; /* P_-2 */
    seq_t prev1 = 1; /* P_-1 */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t coef;
        if (i == 0) {
            coef = 4;
        } else if (i & 1) {          /* odd indices */
            coef = 4;
        } else {                     /* even indices */
            coef = 8;
        }
        seq_t cur = coef * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
