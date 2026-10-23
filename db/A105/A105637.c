/*
 * A105637  a(n) = a(n-2) + a(n-3) - a(n-5), with a(0) = 0, a(1) = 1, a(2) =
 *          2, a(3) = 1, a(4) = 3, a(5) = 3.
 * https://oeis.org/A105637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n/2) - [n mod 3 == 0]
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = 1 + (nn / 2);
    if ((n % 3) == 0) result -= 1;
    return result;
}
