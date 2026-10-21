/*
 * A213575  Antidiagonal sums of the convolution array A213573.
 * https://oeis.org/A213575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + n^3, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t cube = (ii * ii) * ii;   /* i^3 using seq_t arithmetic */
        result = result * 2 + cube;
    }
    return result;
}
