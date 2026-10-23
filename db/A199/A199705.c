/*
 * A199705  Number of -n..n arrays x(0..2) of 3 elements with zero sum and no
 *          two neighbors equal.
 * https://oeis.org/A199705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 3*n^2 for n even a(n) = 2 + n + 3*n^2 for n odd a(1)
 *            = 6, a(2) = 14, … (OEIS A199705)
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but harmless */
    seq_t nn = (seq_t)n;
    seq_t val = 3 * nn * nn + nn;   /* 3*n^2 + n */
    if (n & 1) val += 2;           /* add 2 for odd n */
    return val;
}
