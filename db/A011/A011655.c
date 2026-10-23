/*
 * A011655  Period 3: repeat [0, 1, 1].
 * https://oeis.org/A011655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy, changed
 * Formula:   a(n) = 0 if n mod 3 == 0, otherwise 1 (period 3: [0,1,1])
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 3 == 0) ? (seq_t)0 : (seq_t)1;
}
