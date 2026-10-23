/*
 * A189807  Number of right triangles on an (n+1) X 3 grid.
 * https://oeis.org/A189807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n^2 + 26*n - 42 for n > 3 a(1) = 14, a(2) = 44, a(3) =
 *            94
 * Range:     u8 n <= 5, u16 n <= 102, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)44;
    if (n == 3) return (seq_t)94;

    seq_t nn = (seq_t)n;
    return nn * ((seq_t)6 * nn + (seq_t)26) - (seq_t)42;
}
