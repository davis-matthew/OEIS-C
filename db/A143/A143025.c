/*
 * A143025  Period length 4: repeat [1, 8, 2, 8].
 * https://oeis.org/A143025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n), a(0)=1, a(1)=8, a(2)=2, a(3)=8
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3u) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)8;
        case 2: return (seq_t)2;
        case 3: return (seq_t)8;
    }
    /* unreachable, but required for all control paths */
    return (seq_t)0;
}
