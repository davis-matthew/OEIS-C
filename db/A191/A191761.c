/*
 * A191761  Last digit of (2*n)^2. Also period 5: repeat [0, 4, 6, 6, 4].
 * https://oeis.org/A191761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n*n) mod 10
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* take only the last digit of n to avoid overflow */
    idx_t d0 = n % 10;
    seq_t d = (seq_t)d0;
    seq_t sq = (d * d) % 10;
    seq_t res = (4 * sq) % 10;
    return res;
}
