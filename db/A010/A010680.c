/*
 * A010680  Decimal expansion of 1/11.
 * https://oeis.org/A010680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = 9*(n mod 2)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t nine = (seq_t)9;
    const seq_t parity = (seq_t)(n & 1);
    return parity * nine;
}
