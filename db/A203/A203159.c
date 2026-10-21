/*
 * A203159  (n-1)-st elementary symmetric function of {2,4,6,8,...,2n}.
 * https://oeis.org/A203159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4n-2)*a(n-1) - 4*(n-1)^2*a(n-2) with initial values
 *            a(1)=1, a(2)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    seq_t a_prev2 = (seq_t)1;   /* a(1)   */
    seq_t a_prev  = (seq_t)6;   /* a(2)   */
    seq_t a_curr;

    for (idx_t k = 3; k <= n; ++k) {
        seq_t km1   = (seq_t)(k - 1);
        seq_t term1 = ( ((seq_t)4 * (seq_t)k) - (seq_t)2 ) * a_prev;
        seq_t term2 = (seq_t)4 * km1 * km1 * a_prev2;
        a_curr = term1 - term2;
        a_prev2 = a_prev;
        a_prev  = a_curr;
    }
    return a_prev;
}
