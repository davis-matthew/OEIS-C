/*
 * A095815  a(n) = n + largest digit of n.
 * https://oeis.org/A095815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n + largest digit of n
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t maxd = 0;
    while (x) {
        seq_t d = x % 10;
        if (d > maxd) maxd = d;
        x /= 10;
    }
    return (seq_t)n + maxd;
}
