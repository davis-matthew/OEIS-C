/*
 * A097325  Period 6: repeat [0, 1, 1, 1, 1, 1].
 * https://oeis.org/A097325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n % 6 == 0 else 1
 * Range:     u8 n <= 8692, u16 n <= 8692, u32 n <= 8692, u64 n <= 8692,
 *            u128 n <= 8692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 6 == 0) ? (seq_t)0 : (seq_t)1;
}
