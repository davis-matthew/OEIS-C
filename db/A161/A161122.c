/*
 * A161122  Number of cycles with entries of the same parity in all
 *          fixed-point-free involutions of {1,2,...,2n}.
 * https://oeis.org/A161122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(2n-3)!!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 0 */
    if (n < 2)
        return (seq_t)0;

    /* start with n*(n-1) */
    seq_t res = (seq_t)n * (seq_t)(n - 1);

    /* multiply by the odd double factorial (2n-3)!! */
    seq_t odd = 1;
    for (idx_t i = 0; i <= n - 2; ++i)
        odd *= (seq_t)(2 * i + 1);

    res *= odd;
    return res;
}
