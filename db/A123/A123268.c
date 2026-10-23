/*
 * A123268  X-values of solutions to the equation 3(X-Y)^4 - X*Y = 0 with X >=
 *          Y.
 * https://oeis.org/A123268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c(n)*(1 + d(n)) where c(0) = 0, c(1) = 1, c(n) =
 *            14*c(n-1) - c(n-2) d(0) = 1, d(1) = 7, d(n) = 14*d(n-1) - d(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)                 /* a(0) = 0 */
        return (seq_t)0;

    /* Initial values for the recurrences */
    seq_t c_prev = (seq_t)0;      /* c(0) */
    seq_t c_curr = (seq_t)1;      /* c(1) */
    seq_t d_prev = (seq_t)1;      /* d(0) */
    seq_t d_curr = (seq_t)7;      /* d(1) */

    if (n == 1u)                 /* a(1) = 1*(1+7) = 8 */
        return c_curr * ((seq_t)1 + d_curr);

    /* Iterate using the order‑2 recurrences up to n */
    for (idx_t i = 2u; i <= n; ++i) {
        seq_t c_next = (seq_t)14 * c_curr - c_prev;
        seq_t d_next = (seq_t)14 * d_curr - d_prev;
        c_prev = c_curr;  c_curr = c_next;
        d_prev = d_curr;  d_curr = d_next;
    }

    return c_curr * ((seq_t)1 + d_curr);
}
