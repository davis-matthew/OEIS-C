/*
 * A022126  Fibonacci sequence beginning 3, 16.
 * https://oeis.org/A022126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 16, a(n) = a(n-1) + a(n-2) for n >= 2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)16;

    seq_t a0 = (seq_t)3;
    seq_t a1 = (seq_t)16;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a0 + a1;
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}
