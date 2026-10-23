/*
 * A047055  Quintuple factorial numbers: a(n) = Product_{k=0..n-1} (5*k + 2).
 * https://oeis.org/A047055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (5*k + 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        res *= (seq_t)(5 * i + 2);
    }
    return res;
}
