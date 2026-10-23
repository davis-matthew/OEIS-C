/*
 * A185691  Fibonacci sequence with initial terms 10 and 21.
 * https://oeis.org/A185691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 10, a(1) = 21 (Fibonacci with
 *            custom start)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)21;

    seq_t a0 = (seq_t)10;
    seq_t a1 = (seq_t)21;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = a0 + a1;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
