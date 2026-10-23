/*
 * A109613  Odd numbers repeated.
 * https://oeis.org/A109613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/2)+1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(n/2) * 2 + 1, result fits in seq_t when it does in the
       test regime; cast to the target type afterwards. */
    return (seq_t)((n / 2) * 2 + 1);
}
