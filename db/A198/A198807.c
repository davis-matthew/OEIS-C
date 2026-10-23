/*
 * A198807  Number of closed paths of length 13n whose steps are 13th roots of
 *          unity.
 * https://oeis.org/A198807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*n)!/(n!)^13
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* Product of the 13 consecutive integers from 13(k-1)+1 to 13k */
        seq_t numerator = (seq_t)1;
        seq_t start = (seq_t)(13 * k - 12);             /* 13*(k-1)+1 */
        for (idx_t i = 0; i < 13; ++i) {
            numerator *= (start + (seq_t)i);
        }

        /* Compute k^13 */
        seq_t denom = (seq_t)1;
        seq_t kk = (seq_t)k;
        for (idx_t i = 0; i < 13; ++i) {
            denom *= kk;
        }

        /* Update the running product: result *= numerator/denom */
        result = (result * numerator) / denom;
    }
    return result;
}
