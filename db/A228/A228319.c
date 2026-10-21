/*
 * A228319  The hyper-Wiener index of the graph obtained by applying
 *          Mycielski's construction to the star graph K(1,n).
 * https://oeis.org/A228319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 7*n + 7 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0, return 0 */
    seq_t m = (seq_t)n;
    return 6 * m * m + 7 * m + 7;
}
