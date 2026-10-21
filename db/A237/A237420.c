/*
 * A237420  If n is odd, then a(n) = 0; otherwise, a(n) = n.
 * https://oeis.org/A237420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n if n even, 0 if n odd (A237420)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1)   /* odd index */
        return 0;
    return (seq_t)n;
}
