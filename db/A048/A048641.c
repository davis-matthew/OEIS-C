/*
 * A048641  Partial sums of A003188 (Gray code).
 * https://oeis.org/A048641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=0}^{n-1} (j ^ (j >> 1))
 * Range:     u8 n <= 21, u16 n <= 332, u32 n <= 9875, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the partial sum of the binary reflected Gray code sequence. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 0; j < n; ++j)
    {
        sum += (seq_t)(j ^ (j >> 1));
    }
    return sum;
}
