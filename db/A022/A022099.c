/*
 * A022099  Fibonacci sequence beginning 1, 9.
 * https://oeis.org/A022099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)9;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = a0 + a1;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
