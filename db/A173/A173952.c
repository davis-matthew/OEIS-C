/*
 * A173952  a(1)=32 and, for n > 1, a(n) = 9*a(n-1) + 32.
 * https://oeis.org/A173952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1)+32 with a(1)=32
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)32;
    seq_t val = (seq_t)32;
    for (idx_t i = 2; i <= n; ++i)
        val = val * 9 + 32;
    return val;
}
