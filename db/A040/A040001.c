/*
 * A040001  1 followed by {1, 2} repeated.
 * https://oeis.org/A040001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, frac
 * Formula:   a(0)=1; a(1)=1; for n≥2: a(n)=2 if n is even, 1 if n is odd
 * Range:     u8 n <= 9993, u16 n <= 9993, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    return ((n & 1) == 0) ? 2 : 1;
}
