/*
 * A126562  Number of intersections of at least four edges in a cube of n X n
 *          X n smaller cubes.
 * https://oeis.org/A126562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n-1)^2 + (n-1)^3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* as n >= 1 by OEIS definition */
    seq_t m      = (seq_t)n - 1;   /* m = n-1 */
    seq_t m_sq   = m * m;          /* (n-1)^2 */
    seq_t m_cu   = m_sq * m;       /* (n-1)^3 */
    return (seq_t)6 * m_sq + m_cu; /* 6*(n-1)^2 + (n-1)^3 */
}
