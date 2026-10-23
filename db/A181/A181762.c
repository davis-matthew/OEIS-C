/*
 * A181762  a(n) = n/2 if n is even, otherwise 3n+5.
 * https://oeis.org/A181762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is even) ? n/2 : 3n + 5
 * Range:     u8 n <= 84, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {
        return sn >> 1;      /* sn / 2 for even n */
    } else {
        return sn * 3 + 5;   /* 3n + 5 for odd n */
    }
}
