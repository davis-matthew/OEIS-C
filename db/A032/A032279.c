/*
 * A032279  Number of bracelets (turnover necklaces) of n beads of 2 colors, 5
 *          of them black.
 * https://oeis.org/A032279
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(( (n-5)^4 )/240 + (n-5)^3/24 + 5*(n-5)^2/24 * +
 *            25*(n-5)/48 + 1 + (-1)^(n-5)*(n-5)/16 ) for n ≥ 5 * (This is
 *            derived from a(n+5) = floor(n^4/240 + n^3/24 + 5n^2/24 + 25n/48
 *            * + 1 + (-1)^n*n/16).)
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 313, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. Behaviour here is irrelevant */
    if (n < 5) return 0;

    /* m = n - 5 */
    idx_t m = n - 5;
    seq_t m_se = (seq_t)m;

    /* Compute numerator with common denominator 240:
     *   N = m^4 + 10*m^3 + 10*5*m^2 + 5*25*m + 240 + ((-1)^m)*15*m
     *   a(n) = N / 240
     */
    seq_t term1 = m_se * m_se * m_se * m_se;              /* m^4          */
    seq_t term2 = m_se * m_se * m_se * 10;                /* 10*m^3       */
    seq_t term3 = m_se * m_se * 5 * 10;                   /* 10*5*m^2     */
    seq_t term4 = m_se * 25 * 5;                          /* 5*25*m       */
    seq_t term5 = 240;                                    /* constant 240 */

    seq_t total = term1 + term2 + term3 + term4 + term5;

    if ((m & 1) == 0) {          /* m even -> add (+15*m)   */
        total += m_se * 15;
    } else {                     /* m odd -> subtract (15*m) */
        total -= m_se * 15;
    }

    return total / 240;           /* floor division by 240    */
}
