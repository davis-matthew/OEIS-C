/*
 * A213748  Principal diagonal of the convolution array A213747.
 * https://oeis.org/A213748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - 2^n + 4^n*(n-1) + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power2 = 1;          /* 2^n  */
    seq_t power4 = 1;          /* 4^n  */
    for (idx_t i = 0; i < n; ++i) {
        power2 <<= 1;          /* multiply by 2 */
        power4 <<= 2;          /* multiply by 4 */
    }
    seq_t result = 2;          /* constant 2 */
    result -= power2;          /* subtract 2^n */
    result += power4 * ((seq_t)(n - 1));  /* add 4^n*(n-1) */
    result += (seq_t)n;        /* add n */
    return result;
}
