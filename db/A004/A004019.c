/*
 * A004019  a(0) = 0; for n > 0, a(n) = (a(n-1) + 1)^2.
 * https://oeis.org/A004019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 0; for n > 0, a(n) = (a(n-1) + 1)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    for (idx_t i = 1; i <= n; i++) {
        seq_t temp = result + (seq_t)1;
        result = temp * temp;
    }
    return result;
}
