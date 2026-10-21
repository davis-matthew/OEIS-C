/*
 * A052515  Number of ordered pairs of complementary subsets of an n-set with
 *          both subsets of cardinality at least 2.
 * https://oeis.org/A052515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 4 * a(n) = 2^n - 2*n - 2 for n >= 4 * (Number
 *            of binary strings of length n with at least two 0’s and two
 *            1’s.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n < 4) {
        return (seq_t)0;
    }

    /* Compute 2^n: start from 1 and shift left n times. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 <<= 1;
    }

    /* Compute 2*n */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* a(n) = 2^n - 2*n - 2 */
    pow2 -= two_n;
    pow2 -= (seq_t)2;

    return pow2;
}
