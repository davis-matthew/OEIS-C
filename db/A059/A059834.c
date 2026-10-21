/*
 * A059834  Sum of squares of entries of Wilkinson's eigenvalue test matrix of
 *          order 2n+1.
 * https://oeis.org/A059834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 3*n^2 + 13*n)/3
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Promote index to seq_t for safe arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */

    /* Numerator: 2*n^3 + 3*n^2 + 13*n */
    seq_t num = (seq_t)2 * n3 + (seq_t)3 * n2 + (seq_t)13 * nn;

    /* The division by 3 is exact for all non‑negative integers n */
    return num / 3;
}
