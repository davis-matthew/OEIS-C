/*
 * A059139  A hierarchical sequence (W2{2}*c - see A059126).
 * https://oeis.org/A059139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=4; a(2n)=4; a(2n+1)=a(n)+3
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 4;
    /* For each trailing odd step while shifting right, add 3 to the result */
    while (n & 1) {
        val += 3;
        n >>= 1;
    }
    return val;
}
