/*
 * A063524  Characteristic function of 1.
 * https://oeis.org/A063524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = mu(n^2) (equals 1 only for n = 1, otherwise 0)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 122 terms through n = 9783 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return n == 1 ? (seq_t)1 : (seq_t)0;
}
