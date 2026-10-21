/*
 * A200258  a(n) = Fibonacci(8n+7) mod Fibonacci(8n+1).
 * https://oeis.org/A200258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - a(n-2), with a(1)=32, a(2)=1508
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)1508;

    seq_t a1 = (seq_t)32;
    seq_t a2 = (seq_t)1508;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)47 * a2 - a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
