/*
 * A022359  Fibonacci sequence beginning 0, 25.
 * https://oeis.org/A022359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2), a(0)=0, a(1)=25
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 0;
    seq_t a1 = 25;
    if (n == 0) return a0;
    if (n == 1) return a1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a0 + a1;
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}
