/*
 * A063489  a(n) = (2*n-1)*(5*n^2-5*n+6)/6.
 * https://oeis.org/A063489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(5*n^2-5*n+6)/6
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, but safe to handle n==0 */

    seq_t nn = (seq_t)n;
    seq_t part1 = 2 * nn - 1;
    seq_t part2 = 5 * nn * nn - 5 * nn + 6;
    return part1 * part2 / 6;
}
