/*
 * A084947  a(n) = Product_{i=0..n-1} (7*i+2).
 * https://oeis.org/A084947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{i=0..n-1} (7*i+2) Recurrence: a(0) = 1; a(n) =
 *            a(n-1) * (7*n - 5) for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prod = (seq_t)1;
    /* multiply (7*i+2) for i = 0 .. n-1 */
    for (idx_t i = 0; i < n; ++i)
    {
        prod *= (seq_t)(7 * i + 2);
    }
    return prod;
}
