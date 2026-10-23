/*
 * A175826  Partial sums of ceiling(n^2/8).
 * https://oeis.org/A175826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(2*n^2 + n + 27)/48) (which equals
 *            sum_{k=0}^n ceil(k^2/8) )
 * Range:     u8 n <= 15, u16 n <= 115, u32 n <= 860, u64 n <= 860,
 *            u128 n <= 860, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ii   = (seq_t)i * (seq_t)i;      /* i^2 */
        seq_t term = (ii + 7) / 8;              /* ceil(i^2/8) */
        sum += term;
    }
    return sum;
}
