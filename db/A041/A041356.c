/*
 * A041356  Numerators of continued fraction convergents to sqrt(192).
 * https://oeis.org/A041356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for a(n): numerators of continued‑fraction convergents to sqrt(192).
   a(0) = 13, a(1) = 14.
   For n ≥ 2:
     a(n) = coeff(n) * a(n-1) + a(n-2),
   where the coefficient coeff(n) follows the repeating pattern
   [1, 5, 1, 26] for n ≥ 1 (coeff(1)=1, coeff(2)=5, coeff(3)=1, coeff(4)=26,
   coeff(n+4)=coeff(n)). */

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0;   /* a(-2) */
    seq_t p_prev1 = 1;   /* a(-1) */
    seq_t coeff, p_curr;

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            coeff = (seq_t)13;
        } else {
            switch (i & 3) {        /* i % 4, but faster */
                case 1:
                case 3:
                    coeff = (seq_t)1;
                    break;
                case 2:
                    coeff = (seq_t)5;
                    break;
                case 0:
                    coeff = (seq_t)26;
                    break;
            }
        }
        p_curr = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_prev1;
}
