/*
 * A161124  Number of inversions in all fixed-point-free involutions of
 *          {1,2,...,2n}.
 * https://oeis.org/A161124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * (2n-1)!! (odd double factorial)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        result *= (seq_t)(2 * k - 1);   /* multiply by odd term (2k-1) */
    }
    result *= (seq_t)n;                 /* multiply by n twice */
    result *= (seq_t)n;
    return result;
}
