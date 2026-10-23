/*
 * A034166  Maximum length of 'zig-zag' self avoiding walk on an n X n lattice
 *          from a corner to opposite one.
 * https://oeis.org/A034166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, walk
 * Formula:   a(1)=0 a(2)=2 a(4)=10 For odd n≥3: let k = (n-1)/2; a(n) = 4*k*k
 *            – 4 if k is even, 4*k*k if k is odd For even n≥6: let m = n/2;
 *            a(n) = 4*m*m – 4*m + 2 if m is odd, 4*m*m – 4*m – 2 if m is even
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0 */
        return 0;
    if (n == 2)                /* a(2) = 2 */
        return 2;
    if (n == 4)                /* a(4) = 10 (special case) */
        return 10;

    if ((n & 1) != 0)          /* odd n */
    {
        idx_t k = (n - 1) >> 1;            /* k = (n-1)/2 */
        seq_t k_sq = (seq_t)k * (seq_t)k;
        if ((k & 1) == 0)                 /* k even */
            return 4 * k_sq - 4;
        else                               /* k odd  */
            return 4 * k_sq;
    }
    else                        /* even n */
    {
        idx_t m = n >> 1;                 /* m = n/2 */
        seq_t m_sq = (seq_t)m * (seq_t)m;
        if ((m & 1) != 0)                 /* m odd */
            return 4 * m_sq - 4 * m + 2;
        else                               /* m even (m ≥ 2) */
            return 4 * m_sq - 4 * m - 2;
    }
}
