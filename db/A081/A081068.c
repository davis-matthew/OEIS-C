/*
 * A081068  a(n) = (Lucas(4*n+2) + 2)/5, or Fibonacci(2*n+1)^2, or
 *          A081067(n)/5.
 * https://oeis.org/A081068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) with a(0) = 1, a(1) = 4,
 *            a(2) = 25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS sequence A081068 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)25;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)25;
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 8 * a2;
        next -= 8 * a1;
        next += a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
