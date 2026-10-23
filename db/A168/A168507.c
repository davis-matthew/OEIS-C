/*
 * A168507  a(n) = n^5*(n^10 + 1)/2.
 * https://oeis.org/A168507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 * (n^10 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 20, u128 n <= 388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2   */
    seq_t n3 = n2 * nn;           /* n^3   */
    seq_t n5 = n3 * n2;           /* n^5   */
    seq_t n10 = n5 * n5;          /* n^10  */
    seq_t part = n10 + (seq_t)1;  /* n^10 + 1 */

    /* Reduce the factor that is even to avoid overflow */
    if ((n5 & (seq_t)1) == 0) {
        n5 >>= 1;                 /* n^5 / 2 */
    } else {
        part >>= 1;                /* (n^10 + 1) / 2 */
    }

    return n5 * part;             /* final result */
}
