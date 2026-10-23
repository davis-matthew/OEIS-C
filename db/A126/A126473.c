/*
 * A126473  Number of strings over a 5 symbol alphabet with adjacent symbols
 *          differing by three or less.
 * https://oeis.org/A126473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2); a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t a0 = 1;
    seq_t a1 = 5;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = 4 * a1 + 3 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
