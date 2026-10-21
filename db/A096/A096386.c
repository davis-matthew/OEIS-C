/*
 * A096386  Number of numbers <= n which are divisible by 2 or 3.
 * https://oeis.org/A096386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n/2] + [n/3] - [n/6] (number of integers ≤ n divisible
 *            by 2 or 3)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn / 2 + nn / 3 - nn / 6;
}
