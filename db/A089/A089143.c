/*
 * A089143  a(n) = 9*2^n - 6.
 * https://oeis.org/A089143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^n - 6 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n iteratively to avoid overflow on large n */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= (seq_t)2;
    }

    seq_t ans = pow * (seq_t)9;
    ans -= (seq_t)6;
    return ans;
}
