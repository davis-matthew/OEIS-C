/*
 * A237036  Size of the smallest conjugacy class of size greater than 1 of the
 *          alternating group of degree n.
 * https://oeis.org/A237036
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*C(n,3) for n >= 9, special values for n = 4..8: a(4) =
 *            3, a(5) = 12, a(6) = 40, a(7) = 70, a(8) = 105 (for n < 4 the
 *            result is undefined – return 0)
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)12;
    if (n == 6) return (seq_t)40;
    if (n == 7) return (seq_t)70;
    if (n == 8) return (seq_t)105;

    /* n >= 9: compute 2 * n * (n-1) * (n-2) / 6 */
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn - 1) * (nn - 2);
    val /= 6;
    val *= 2;
    return val;
}
