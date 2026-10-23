/*
 * A174766  y-values in the solution to x^2 - 22*y^2 = 1.
 * https://oeis.org/A174766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 394*a(n-1) - a(n-2), a(1)=0, a(2)=42
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 42;

    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 42;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)394 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
