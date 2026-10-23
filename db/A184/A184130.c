/*
 * A184130  Number of n X 2 0..2 arrays with rows and columns in nondecreasing
 *          order.
 * https://oeis.org/A184130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (40320 + 79344*n + 64140*n^2 + 37884*n^3 + 16009*n^4 +
 *            3696*n^5 + 490*n^6 + 36*n^7 + n^8) / 40320 A184130: Number of
 *            n×2 arrays with entries 0..2, each row and column in
 *            nondecreasing order
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Horner's rule applied to the degree‑8 polynomial */
    seq_t res = (seq_t)1;          /* start with coefficient of n^8   */
    res = res * (seq_t)n + 36;     /* + 36·n^7 */
    res = res * (seq_t)n + 490;    /* + 490·n^6 */
    res = res * (seq_t)n + 3696;   /* + 3696·n^5 */
    res = res * (seq_t)n + 16009;  /* + 16009·n^4 */
    res = res * (seq_t)n + 37884;  /* + 37884·n^3 */
    res = res * (seq_t)n + 64140;  /* + 64140·n^2 */
    res = res * (seq_t)n + 79344;  /* + 79344·n   */
    res = res * (seq_t)n + 40320;  /* + 40320     */
    return res / (seq_t)40320;     /* final division */
}
