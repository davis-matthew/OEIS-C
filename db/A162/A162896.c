/*
 * A162896  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(0) = 1, a(1) = 47, a(2) =
 *            2162, a(3) = 98371, and for n >= 4: a(n) = 45*a(n-1) + 45*a(n-2)
 *            - 1035*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)47;
    if (n == 2) return (seq_t)2162;
    if (n == 3) return (seq_t)98371;

    /* Use a rolling buffer for the last three terms */
    seq_t a0 = (seq_t)1;          /* a(n-3)   */
    seq_t a1 = (seq_t)47;         /* a(n-2)   */
    seq_t a2 = (seq_t)2162;       /* a(n-1)   */
    seq_t a3 = (seq_t)98371;      /* a(n)     */

    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)( (seq_t)45 * a3 + (seq_t)45 * a2 - (seq_t)1035 * a1 );
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
