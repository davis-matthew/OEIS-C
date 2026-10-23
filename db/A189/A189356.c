/*
 * A189356  a(n) gives y-values solving the Diophantine equation 2*x^2 +
 *          (x-1)^2 = y^2 for positive x.
 * https://oeis.org/A189356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2), a(1) = 3, a(2) = 41
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)41;

    seq_t prev2 = (seq_t)3;   /* a(1) */
    seq_t prev1 = (seq_t)41;  /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = ((seq_t)14 * prev1) - prev2;  /* a(i) = 14*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;  /* a(n) */
}
