/*
 * A060633  Surround numbers of an n X 1 rectangle.
 * https://oeis.org/A060633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 + 22*n^3 + 105*n^2 - 56*n - 8)/4
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 289, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;          /* n^2 */
    seq_t n3 = n2 * x;         /* n^3 */
    seq_t n4 = n3 * x;         /* n^4 */
    seq_t num = n4 + (seq_t)22 * n3 + (seq_t)105 * n2 - (seq_t)56 * x - (seq_t)8;
    return num / (seq_t)4;
}
