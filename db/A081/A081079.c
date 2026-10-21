/*
 * A081079  a(n) = Lucas(4*n+2) - 3 = 5*Fibonacci(2*n)*Fibonacci(2*n+2).
 * https://oeis.org/A081079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4*n+2) - 3 Equivalent recurrence (with offset 0):
 *            a(0)=0, a(1)=15, a(2)=120 a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3)
 *            for n >= 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)120;

    seq_t a0 = 0;           // a(n-3)
    seq_t a1 = 15;          // a(n-2)
    seq_t a2 = 120;         // a(n-1)
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = 8 * a2 - 8 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;              // a(n)
}
