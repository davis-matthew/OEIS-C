/*
 * A140486  Trajectory of 2 under repeated application of the map: n -> n +
 *          second-smallest number that does not divide n.
 * https://oeis.org/A140486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; * for n ≥ 2, a(n) = a(n-1) + m2, * where m2 is the
 *            second smallest integer m ≥ 2 that does not divide a(n-1)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, safe for n ≥ 1 */
    seq_t current = 2;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t m1 = 0, m2 = 0;
        for (seq_t m = 2; ; ++m)
        {
            if (current % m != 0)
            {
                if (m1 == 0)
                    m1 = m;
                else
                {
                    m2 = m;
                    break;
                }
            }
        }
        current += m2;
    }
    return current;
}
