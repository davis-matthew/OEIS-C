/*
 * A052578  a(0) = 0, a(n) = 4*n! for n > 0.
 * https://oeis.org/A052578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * n! for n > 0, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined explicitly. */
    if (n == 0) {
        return 0;
    }

    /* Compute 4 * n! iteratively.
       For n = 1, the loop is skipped and the result is 4. */
    seq_t result = (seq_t)4;
    for (idx_t i = 2; i <= n; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
