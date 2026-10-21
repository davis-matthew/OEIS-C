/*
 * A099013  a(n) = Sum_{k=0..n} 3^(k-1)*Fibonacci(k).
 * https://oeis.org/A099013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) - 9*a(n-3) with initial values
 *            a(0)=0, a(1)=1, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)0, a1 = (seq_t)1, a2 = (seq_t)4, ai;
    for (idx_t i = 3; i <= n; ++i) {
        /* compute ai = 4*a2 + 6*a1 - 9*a0 */
        ai = (seq_t)4 * a2 + (seq_t)6 * a1 - (seq_t)9 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
