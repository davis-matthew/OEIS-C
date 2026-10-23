/*
 * A015017  q-factorial numbers for q=-4.
 * https://oeis.org/A015017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{k=1..n} ((-4)^k - 1) / (-4 - 1) Recurrence: a(0)=1;
 *            a(n)= (((-4)^n - 1) * a(n-1)) / (-5)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t a_val = 1;          /* holds a(k-1) during iteration     */
    seq_t pow    = 1;          /* holds (-4)^(k-1), start with k=1   */
    for (idx_t k = 1; k <= n; ++k) {
        pow = pow * (seq_t)(-4);           /* (-4)^k      */
        seq_t term = pow - (seq_t)1;       /* (-4)^k - 1  */
        a_val = (term * a_val) / (seq_t)(-5);
    }
    return a_val;
}
