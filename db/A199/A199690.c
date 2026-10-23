/*
 * A199690  a(n) = (11*10^n + 1)/3.
 * https://oeis.org/A199690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*10^n + 1)/3 Recurrence: a(0) = 4; a(n) = 10*a(n-1) -
 *            3 (for n >= 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    seq_t res = 4;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 10 - 3;
    }
    return res;
}
