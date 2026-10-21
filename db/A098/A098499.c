/*
 * A098499  Number of squares on infinite half chessboard at <=n knight moves
 *          from a fixed point on the diagonal.
 * https://oeis.org/A098499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 a(n) = 5 for n = 1 a(n) = 23 for n = 2 a(n) =
 *            57 for n = 3 a(n) = (28⋅n² - 6⋅n + 9 + 3⋅(−1)ⁿ) / 4 for n > 3
 *            (the division is exact for all integer n)
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)23;
    if (n == 3) return (seq_t)57;

    seq_t ns = (seq_t)n;
    seq_t val = (seq_t)28 * ns * ns;         /* 28 n² */
    val -= (seq_t)6 * ns;                   /* −6 n */
    seq_t delta = (ns & (seq_t)1) ? (seq_t)6 : (seq_t)12;   /* 9 + 3(−1)ⁿ */
    val += delta;
    return val / (seq_t)4;
}
