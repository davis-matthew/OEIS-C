/*
 * A040341  Continued fraction for sqrt(360).
 * https://oeis.org/A040341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 18, a(1) = 1, a(2) = 36, a(n) = a(n-2) for n >= 3 (OEIS
 *            A040341)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)18;

    /* Even indices (except 0) give 36, odd give 1 */
    if ((n & 1) == 0)
        return (seq_t)36;
    else
        return (seq_t)1;
}
