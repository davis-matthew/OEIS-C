/*
 * A211381  Number of pairs of intersecting diagonals in the exterior of a
 *          regular n-gon.
 * https://oeis.org/A211381
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-4)*(n-6)*(2*n-7)/24 for n even; a(n) =
 *            n*(n-3)*(n-5)*(2*n-11)/24 for n odd
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6) return 0;                /* values a(3) … a(6) are zero */

    if ((n & 1) == 0) {                  /* even n (n >= 8) */
        seq_t term = (seq_t)n *
                     (seq_t)(n - 4) *
                     (seq_t)(n - 6) *
                     ((seq_t)n * (seq_t)2 - (seq_t)7);
        return term / (seq_t)24;
    } else {                             /* odd n (n >= 7) */
        seq_t term = (seq_t)n *
                     (seq_t)(n - 3) *
                     (seq_t)(n - 5) *
                     ((seq_t)n * (seq_t)2 - (seq_t)11);
        return term / (seq_t)24;
    }
}
