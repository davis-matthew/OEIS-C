/*
 * A122961  Alternately form product and sum of all previous terms.
 * https://oeis.org/A122961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; for n > 1, a(2n-1) = a(2n-3)^2 * a(2n-2), a(2n)
 *            = 2 * a(2n-2) + a(2n-1);
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    seq_t odd   = (seq_t)1;  /* a(1)  */
    seq_t even  = (seq_t)1;  /* a(2)  */

    for (idx_t k = 2; ; ++k)
    {
        seq_t newOdd  = odd * odd * even;
        seq_t newEven = even * (seq_t)2 + newOdd;

        if ((2 * k - 1) == n) return newOdd;
        if ((2 * k)     == n) return newEven;

        odd  = newOdd;
        even = newEven;
    }
}
