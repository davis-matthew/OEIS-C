/*
 * A131509  a(n) = (n + 1)*(n^2 + 2)*(n^3 + 3)/6.
 * https://oeis.org/A131509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n^2 + 2)*(n^3 + 3)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 41, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote the index to the sequence type to keep all arithmetic in
     * seq_t.  Arithmetic is performed using only +, -, *, /, %,
     * comparisons, and bitwise ops, as required.  The division by 6 is
     * exact because the expression is integer‑valued for all n. */
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;          /* n^2 */
    seq_t N3  = N2 * N;         /* n^3 */

    seq_t t1  = N + 1;          /* n + 1 */
    seq_t t2  = N2 + 2;         /* n^2 + 2 */
    seq_t t3  = N3 + 3;         /* n^3 + 3 */

    seq_t prod = t1 * t2;       /* (n+1)*(n^2+2) */
    prod = prod * t3;           /* * (n^3+3) */

    prod /= 6;                  /* divide by 6 */

    return prod;
}
