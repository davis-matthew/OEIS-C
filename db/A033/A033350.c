/*
 * A033350  a(n) = floor(30/n).
 * https://oeis.org/A033350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(30/n)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1, but guard against zero just in case */
    if (n == 0) {
        return (seq_t)0;
    }
    /* 30 is a small constant; the division is integer division so it
       yields floor(30/n) for unsigned arithmetic. The result fits in
       seq_t because it is ≤ 30. */
    return (seq_t)(30 / n);
}
