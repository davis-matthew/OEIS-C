/*
 * A063524  Characteristic function of 1.
 * https://oeis.org/A063524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = mu(n^2) (equals 1 only for n = 1, otherwise 0)
 * Range:     u8 n <= 9783, u16 n <= 9783, u32 n <= 9783, u64 n <= 9783,
 *            u128 n <= 9783, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return n == 1 ? (seq_t)1 : (seq_t)0;
}
