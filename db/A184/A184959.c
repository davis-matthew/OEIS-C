/*
 * A184959  Fibonacci sequence beginning 10, 9.
 * https://oeis.org/A184959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0)=10, a(1)=9
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)9;

    seq_t a0 = (seq_t)10;
    seq_t a1 = (seq_t)9;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = a0 + a1;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
