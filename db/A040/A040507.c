/*
 * A040507  Continued fraction for sqrt(531).
 * https://oeis.org/A040507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 23, for n ≥ 1, a(n) = 23 if n is odd, otherwise a(n) = 46
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;
    /* For n ≥ 1 */
    if ((n & 1ULL) == 1ULL)
        return (seq_t)23;
    else
        return (seq_t)46;
}
