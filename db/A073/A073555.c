/*
 * A073555  Number of Fibonacci numbers F(k), k <= 10^n, which end in 8.
 * https://oeis.org/A073555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (10^n + 20) / 15 for n > 1 Recurrence used for efficient
 *            computation: a(1) = 1, a(2) = 8 a(n) = 10 * a(n-1) – 12 for n >=
 *            3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour for n<1 is undefined */
    seq_t a1 = 1;                  /* a(1) */
    if (n == 1) return a1;
    seq_t a2 = 8;                  /* a(2) */
    if (n == 2) return a2;

    seq_t prev = a1;  /* a(n-2) */
    seq_t curr = a2;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = curr * 10 - 12;  /* a(i) = 10*a(i-1) – 12 */
        prev = curr;
        curr = next;
    }
    return curr;   /* a(n) */
}
