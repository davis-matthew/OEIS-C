/*
 * A198858  a(n) = 11*8^n-1.
 * https://oeis.org/A198858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*8^n - 1 Recurrence: a(0) = 10, a(n) = 8*a(n-1) + 7 for
 *            n > 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 11 * 8^n, then subtract 1 */
    seq_t prod = (seq_t)11;
    for (idx_t i = 0; i < n; ++i)
        prod *= (seq_t)8;
    prod -= (seq_t)1;
    return prod;
}
