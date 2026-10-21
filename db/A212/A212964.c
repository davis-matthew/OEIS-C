/*
 * A212964  Number of (w,x,y) with all terms in {0,...,n} and |w-x| < |x-y| <
 *          |y-w|.
 * https://oeis.org/A212964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} floor(k^2/2) - 2*floor(k/2)
 * Range:     u8 n <= 12, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This directly implements the summation formula from OEIS A212964. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t floor_sq   = (kk * kk) / 2;  /* floor(k^2 / 2) */
        seq_t floor_half = kk / 2;         /* floor(k / 2)   */
        sum += floor_sq - 2 * floor_half;
    }
    return sum;
}
