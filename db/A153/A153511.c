/*
 * A153511  a(n) = 4 * A051189(n).
 * https://oeis.org/A153511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * 8^n * n! (i.e. a(n) = 4 * A051189(n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 4;
    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (seq_t)8 * (seq_t)i;   /* multiply by 8*i each step */
    }
    return res;
}
