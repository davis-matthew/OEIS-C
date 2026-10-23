/*
 * A167873  Period 4: repeat [10, 6, 10, 4].
 * https://oeis.org/A167873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10 if n mod 4 = 1 or 3, 6 if n mod 4 = 2, 4 if n mod 4 =
 *            0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[4] = {(seq_t)10, (seq_t)6, (seq_t)10, (seq_t)4};
    return table[(n - 1) & 3];
}
