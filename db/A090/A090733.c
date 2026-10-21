/*
 * A090733  a(n) = 25*a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 25.
 * https://oeis.org/A090733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 25*a(n-1) - a(n-2), a(0) = 2, a(1) = 25
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)25;

    seq_t prev2 = 2;
    seq_t prev1 = 25;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)25 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
