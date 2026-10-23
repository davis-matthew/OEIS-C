/*
 * A157729  a(n) = Fibonacci(n) + 5.
 * https://oeis.org/A157729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) + 5 a(0) = 5, a(1) = 6 Recurrence: a(n) =
 *            a(n-2) + a(n-1) - 5 for n >= 2
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;
    if (n == 1)
        return (seq_t)6;

    seq_t a0 = (seq_t)5;   /* a(0) */
    seq_t a1 = (seq_t)6;   /* a(1) */
    seq_t an = 0;

    for (idx_t i = 2; i <= n; ++i) {
        an = a0 + a1;
        an -= (seq_t)5;
        a0 = a1;
        a1 = an;
    }
    return an;
}
