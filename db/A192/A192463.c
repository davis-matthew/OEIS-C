/*
 * A192463  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) = (x+1) * (2x+1) * ... *(nx+1).
 * https://oeis.org/A192463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A192463
    Coefficient of x in the reduction of
    p(n,x) = ∏_{k=1}^n (k x + 1)
    by the relation x^2 → x + 1.
    The reduced form of any polynomial is always linear:  a + b x.
    This function computes b by iterating over the factors.
*/

seq_t a(idx_t n)
{
    /* a_coeff + b_coeff * x      (start with 1) */
    seq_t a_coeff = 1;
    seq_t b_coeff = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;

        /* Multiply current (a_coeff + b_coeff x)
         * by the new factor (1 + k x) and reduce x^2 → x + 1 */
        seq_t new_a = a_coeff + b_coeff * k_seq;
        seq_t new_b = a_coeff * k_seq + b_coeff + b_coeff * k_seq;

        a_coeff = new_a;
        b_coeff = new_b;
    }
    return b_coeff;
}
