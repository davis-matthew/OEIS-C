/*
 * A071246  a(n) = n*(n - 1)*(2*n^2 + n + 2)/6.
 * https://oeis.org/A071246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(2*n^2 + n + 2)/6
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 271, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* The expression below works for all n ≥ 0 even when n = 0 or 1
       because unsigned arithmetic wraps around harmlessly. */
    seq_t result = sn * (sn - 1) *
                   (((seq_t)2 * sn * sn) + sn + (seq_t)2) / (seq_t)6;
    return result;
}
