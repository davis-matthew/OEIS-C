/*
 * A026488  a(n) is the least positive integer > a(n-1) and not a(i)*a(j)-a(k)
 *          for 1 <= i <= j <= k <= n, where a(1) = 1.
 * https://oeis.org/A026488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 3, and a(n) = 3*n - 5 for n >= 3
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the conjectured closed form of OEIS A026488. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 3;
    return (seq_t)3 * (seq_t)n - 5;
}
