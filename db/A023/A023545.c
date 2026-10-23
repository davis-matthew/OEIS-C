/*
 * A023545  Convolution of natural numbers >= 2 and natural numbers >= 3.
 * https://oeis.org/A023545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 12*n + 23)/6
 * Range:     u8 n <= 8, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    // Compute n^2 + 12*n + 23
    seq_t temp = n_t * n_t + (seq_t)12 * n_t + (seq_t)23;

    /* Cancel the factor 6 early to avoid overflow */
    /* Divide by 2 */
    if ((n_t & (seq_t)1) == 0) {
        n_t >>= 1;
    } else {
        temp /= 2;
    }
    /* Divide by 3 */
    if ((n_t % (seq_t)3) == 0) {
        n_t /= 3;
    } else {
        temp /= 3;
    }
    return n_t * temp;
}
