/*
 * A068636  a(n) = Min(n, R(n)), where R(n) (A004086) = digit reversal of n.
 * https://oeis.org/A068636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look, hear
 * Formula:   a(n) = min(n, reverse(n))
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rev = 0;
    idx_t t = n;
    while (t) {
        rev = rev * 10 + (t % 10);
        t   /= 10;
    }
    seq_t ns = (seq_t)n;
    seq_t rv = (seq_t)rev;
    return ns < rv ? ns : rv;
}
