/*
 * A077802  Sum of products of parts increased by 1 in hook partitions of n,
 *          where hook partitions are of the form h*1^(n-h).
 * https://oeis.org/A077802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*2^n - n - 3 for n > 0, a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 2^n by repeated multiplication. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    seq_t result = 3 * pow2 - (seq_t)n - (seq_t)3;
    return result;
}
