/*
 * A074603  a(n) = 2^n + 8^n.
 * https://oeis.org/A074603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1; /* 2^0 */
    seq_t pow8 = 1; /* 8^0 */

    for (idx_t i = 0; i < n; ++i)
    {
        pow2 = pow2 + pow2;      /* multiply by 2 */
        pow8 = pow8 * (seq_t)8;  /* multiply by 8 */
    }

    return pow2 + pow8;
}
