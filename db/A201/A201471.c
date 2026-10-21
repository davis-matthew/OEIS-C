/*
 * A201471  Maximal diameter of a connected n-gamma_t-vertex-critical graph.
 * https://oeis.org/A201471
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3,4,6,7,9,11 for n = 3..8; a(n) = 2*n - 3 for n >= 9
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)                /* undefined for n < 3; return 0 */
        return (seq_t)0;

    if (n <= 8)               /* handle indices 3..8 */
    {
        static const seq_t vals[6] = { (seq_t)3, (seq_t)4, (seq_t)6,
                                       (seq_t)7, (seq_t)9, (seq_t)11 };
        return vals[n - 3];
    }

    /* n >= 9 : a(n) = 2*n - 3 */
    return ((seq_t)n * (seq_t)2) - (seq_t)3;
}
