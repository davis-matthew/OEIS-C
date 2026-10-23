/*
 * A041036  Numerators of continued fraction convergents to sqrt(23).
 * https://oeis.org/A041036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=4, a(1)=5; for n≥2, a(n)=A_n*a(n-1)+a(n-2), where A_n is
 *            the (n+1)-th partial quotient for √23 (periodic pattern 1,3,1,8)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)4;
    const unsigned char pattern[4] = {1, 3, 1, 8};
    seq_t p_prev2 = (seq_t)1; /* p_{-1} */
    seq_t p_prev1 = (seq_t)4; /* p_{0} */
    seq_t p_cur;
    idx_t i;
    for (i = 1; i <= n; ++i) {
        unsigned char ai = pattern[(i - 1) % 4];
        p_cur = (seq_t)ai * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }
    return p_prev1;
}
