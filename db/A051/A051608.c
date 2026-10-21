/*
 * A051608  a(n) = (3*n+8)!!!/8!!!.
 * https://oeis.org/A051608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(0) * Π_{i=1..n} (3*i+8) a(0) = 1 This follows from a(n)
 *            = a(n-1) * (3n+8) for n≥1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute factor (3*i + 8) as seq_t to avoid overflow in idx_t */
        seq_t factor = (seq_t)3 * (seq_t)i + (seq_t)8;
        res *= factor;
    }
    return res;
}
