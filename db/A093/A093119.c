/*
 * A093119  Number of convex polyominoes with a 3 X n+1 minimal bounding
 *          rectangle.
 * https://oeis.org/A093119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093119 – Number of convex polyominoes with a 3 × (n+1) minimal bounding rectangle
   Formula: a(n) = (6*n^4 + 20*n^3 + 27*n^2 + 19*n + 6)/6
   This expression yields an integer for all n ≥ 1 and is computed in O(1) time. */
seq_t a(idx_t n)
{
    seq_t t  = (seq_t)n;       /* t = n */
    seq_t t2 = t * t;          /* n^2 */
    seq_t t3 = t2 * t;         /* n^3 */
    seq_t t4 = t3 * t;         /* n^4 */

    /* Numerator of the polynomial */
    seq_t numerator = (seq_t)6 * t4 + (seq_t)20 * t3 + (seq_t)27 * t2 + (seq_t)19 * t + (seq_t)6;

    return numerator / (seq_t)6;
}
