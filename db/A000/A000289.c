/*
 * A000289  A nonlinear recurrence: a(n) = a(n-1)^2 - 3*a(n-1) + 3 (for n>1).
 * https://oeis.org/A000289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^2 - 3*a(n-1) + 3 for n > 1, a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x;
    idx_t i;
    if (n == 0) return (seq_t)1;
    x = (seq_t)4;
    for (i = 1; i < n; i++)
        x = x * (x - 3) + 3;
    return x;
}
