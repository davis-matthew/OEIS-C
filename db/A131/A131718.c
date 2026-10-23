/*
 * A131718  Period 6: repeat [1, 1, 2, 1, 2, 1].
 * https://oeis.org/A131718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-6) for n>5; a(0)=1, a(1)=1, a(2)=2, a(3)=1, a(4)=2,
 *            a(5)=1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)1
    };
    return pattern[n % 6];
}
