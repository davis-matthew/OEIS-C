/*
 * A157872  a(n) = 9*n^2 - 3.
 * https://oeis.org/A157872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 3
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. If called with n == 0
       the result is undefined, but we return 0 to avoid a crash.
    */
    if (n == 0)
        return 0;

    /* Compute 9*n*n - 3 using only integer arithmetic.  The cast
       of n to seq_t on both sides of the multiplication ensures
       the computation is performed in the target type and
       preserves the intended bits.
    */
    return ((seq_t)n * (seq_t)n * 9u) - 3u;
}
