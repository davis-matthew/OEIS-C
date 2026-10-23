/*
 * A027878  a(n) = Product_{i=1..n} (10^i - 1).
 * https://oeis.org/A027878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} (10^i - 1), with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t pow10   = (seq_t)10;          /* 10^1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = pow10 - (seq_t)1;   /* 10^i - 1 */
        result *= term;                 /* multiply into product */
        pow10   *= (seq_t)10;           /* advance 10^(i+1) */
    }
    return result;
}
