/*
 * A041015  Denominators of continued fraction convergents to sqrt(11).
 * https://oeis.org/A041015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=1, a(1)=3. For n≥2, a(n) = ((n-1)%2==1 ? 6 : 3) * a(n-1) +
 *            a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_prevprev = (seq_t)1; /* a(0) */
    seq_t a_prev     = (seq_t)3; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t coeff = (((i - 1) & 1) ? (seq_t)6 : (seq_t)3);
        seq_t a_cur = coeff * a_prev + a_prevprev;
        a_prevprev = a_prev;
        a_prev     = a_cur;
    }
    return a_prev;
}
