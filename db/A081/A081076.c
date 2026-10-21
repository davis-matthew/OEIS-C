/*
 * A081076  a(n) = Lucas(4n) + 3, or 5*Fibonacci(2n-1)*Fibonacci(2n+1).
 * https://oeis.org/A081076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) Initial values: a(0) = 5,
 *            a(1) = 10, a(2) = 50 (OEIS A081076)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)50;

    /* Compute the recurrence iteratively. */
    seq_t a0 = (seq_t)5;   /* a(n-3) */
    seq_t a1 = (seq_t)10;  /* a(n-2) */
    seq_t a2 = (seq_t)50;  /* a(n-1) */
    seq_t a3;              /* a(n) */
    seq_t eight = (seq_t)8;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = eight * a2 - eight * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;  /* a(n) after the loop */
}
