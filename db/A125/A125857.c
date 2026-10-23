/*
 * A125857  Numbers whose base-9 representation is 22222222.......2.
 * https://oeis.org/A125857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 9*a(n-1) + 2 (a(1) = 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0 */
    if (n == 1) return (seq_t)0;

    seq_t result = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        result = result * (seq_t)9 + (seq_t)2;
    }
    return result;
}
