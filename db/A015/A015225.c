/*
 * A015225  Odd hexagonal pyramidal numbers.
 * https://oeis.org/A015225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (m)*(m+1)*(4*m-1)/6 where m = 2*n-1 if n is odd, and m =
 *            2*n-2 if n is even (i.e. a(n) is the nth odd value of the
 *            hexagonal pyramidal numbers)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 625, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* map the sequence index to the corresponding n in the H(n) formula */
    idx_t m = (n & 1) ? (2 * n - 1) : (2 * n - 2);   /* n mod 4 == 1 or 2 */

    /* perform the calculation with seq_t arithmetic to keep arbitrary precision */
    seq_t m1 = (seq_t)m;
    seq_t m2 = m1 + 1;                     /* m+1 */
    seq_t four_m_minus_1 = (seq_t)4 * m1 - 1; /* 4*m-1 */

    seq_t num = m1 * m2 * four_m_minus_1;  /* numerator: m*(m+1)*(4m-1) */
    seq_t res = num / (seq_t)6;             /* divide by 6 */

    return res;
}
