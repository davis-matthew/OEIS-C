/*
 * A082988  a(n) = Sum_{k=0..n} 4^k*F(k) where F(k) is the k-th Fibonacci
 *          number.
 * https://oeis.org/A082988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 4^k * F(k) (F(k) is the k-th Fibonacci
 *            number). The sequence satisfies the recurrence a(0) = 0, a(1) =
 *            4, a(2) = 20, a(n) = 5·a(n‑1) + 12·a(n‑2) – 16·a(n‑3) for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 20;

    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 4;   /* a(n-2) */
    seq_t a2 = 20;  /* a(n-1) */
    seq_t an;       /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 5*a2 + 12*a1 - 16*a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
