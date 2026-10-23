/*
 * A161738  Sequence related to the column sums of the BG2 matrix.
 * https://oeis.org/A161738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=0}^{⌊n/2-1⌋} (2*n - 3 - 2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the product is empty, defined to be 1 */
    if (n < 2)
        return (seq_t)1;

    idx_t m = n / 2;                 /* number of factors = floor(n/2)   */
    seq_t result = (seq_t)1;
    seq_t two_n = (seq_t)2 * n;       /* 2*n in seq_t arithmetic          */

    for (idx_t i = 0; i < m; ++i) {
        seq_t term = two_n - (seq_t)3 - (seq_t)2 * i;
        result *= term;
    }

    return result;
}
