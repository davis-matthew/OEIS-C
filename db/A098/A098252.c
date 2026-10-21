/*
 * A098252  Chebyshev polynomials S(n,363) + S(n-1,363) with Diophantine
 *          property.
 * https://oeis.org/A098252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 363*a(n-1) - a(n-2), a(0)=1, a(1)=364
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)364;

    /* 2x2 matrix exponentiation:
       | 363  -1 |
       |  1   0 |
       An = [ T^(n-1) ] * [ a(1) ]
                            [ a(0) ]
     */
    seq_t m00 = (seq_t)363;          /* first row, first column   */
    seq_t m01 = (seq_t)-1;          /* first row, second column  */
    seq_t m10 = (seq_t)1;           /* second row, first column  */
    seq_t m11 = (seq_t)0;           /* second row, second column */
    seq_t r00 = (seq_t)1;           /* Identity matrix – row 0, col 0 */
    seq_t r01 = (seq_t)0;           /* Identity matrix – row 0, col 1 */
    seq_t r10 = (seq_t)0;           /* Identity matrix – row 1, col 0 */
    seq_t r11 = (seq_t)1;           /* Identity matrix – row 1, col 1 */

    idx_t exp = n - 1;              /* exponent */

    while (exp > 0) {
        if (exp & 1) {
            /* r = r * m */
            seq_t t00 = r00 * m00 + r01 * m10;
            seq_t t01 = r00 * m01 + r01 * m11;
            seq_t t10 = r10 * m00 + r11 * m10;
            seq_t t11 = r10 * m01 + r11 * m11;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        /* m = m * m */
        seq_t t00 = m00 * m00 + m01 * m10;
        seq_t t01 = m00 * m01 + m01 * m11;
        seq_t t10 = m10 * m00 + m11 * m10;
        seq_t t11 = m10 * m01 + m11 * m11;
        m00 = t00; m01 = t01; m10 = t10; m11 = t11;

        exp >>= 1;
    }

    /* a(n) = r00 * a(1) + r01 * a(0) */
    return r00 * (seq_t)364 + r01 * (seq_t)1;
}
