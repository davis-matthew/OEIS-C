/*
 * A012244  a(n+2) = (2n+3)*a(n+1) + (n+1)^2*a(n), a(0) = 1, a(1) = 1.
 * https://oeis.org/A012244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = (2n+3)*a(n+1) + (n+1)^2*a(n), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known base cases directly. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Two previous terms in the recurrence. */
    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)1;  /* a(1) */

    /* Compute subsequent terms iteratively. */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = (2i-1)*a(i-1) + (i-1)^2*a(i-2) */
        seq_t term1 = ((seq_t)(2 * i - 1)) * a_prev1;
        seq_t term2 = ((seq_t)(i - 1)) * ((seq_t)(i - 1)) * a_prev2;
        seq_t a_i   = term1 + term2;

        /* Shift for next iteration. */
        a_prev2 = a_prev1;
        a_prev1 = a_i;
    }

    /* a_prev1 holds a(n). */
    return a_prev1;
}
