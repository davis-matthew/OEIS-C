/*
 * A102394  A wicked odious sequence.
 * https://oeis.org/A102394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1) if popcount(n) is odd, otherwise 0
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute parity of the number of set bits in n */
    idx_t x = n;
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    /* least significant bit of x now holds the parity (1 if odd) */
    if (x & 1ULL) {
        return (seq_t)(n + 1);
    } else {
        return (seq_t)0;
    }
}
