/*
 * A105638  Maximum number of intersections in self-intersecting n-gon.
 * https://oeis.org/A105638
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3)/2 if n is odd, n*(n-4)/2 + 1 if n is even
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined (offset 3). Return 0 safely. */
    if (n < 3) return 0;
    if (n & 1) {
        /* n odd */
        return (seq_t)n * ((seq_t)n - 3) / 2;
    } else {
        /* n even */
        return (seq_t)n * ((seq_t)n - 4) / 2 + 1;
    }
}
