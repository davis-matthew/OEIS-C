/*
 * A041588  Numerators of continued fraction convergents to sqrt(312).
 * https://oeis.org/A041588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=17, a(1)=18, a(n) = ((n%4==0)?34:1)*a(n-1)+a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)17;          /* a(0) */

    /* p_{-1} = 1, p_0 = 17 */
    seq_t p_prev = (seq_t)1;
    seq_t p_curr = (seq_t)17;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t coeff = (k % 4 == 0) ? (seq_t)34 : (seq_t)1;
        seq_t p_next = coeff * p_curr + p_prev;
        p_prev = p_curr;
        p_curr = p_next;
    }
    return p_curr;
}
