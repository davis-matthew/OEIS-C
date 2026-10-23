/*
 * A185127  a(n) = n 3's sandwiched between two 1's.
 * https://oeis.org/A185127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) = 10*a(n-1)+21 with a(0)=11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence directly generates the integer consisting of
 * a leading 1, followed by n copies of 3, and a trailing 1.  */

seq_t a(idx_t n)
{
    seq_t val = 11;          /* a(0) */
    if (n == 0)
        return val;
    for (idx_t i = 1; i <= n; ++i)
        val = val * 10 + 21; /* a(i) = 10*a(i-1)+21 */
    return val;
}
