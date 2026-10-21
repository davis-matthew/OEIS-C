/*
 * A095264  a(n) = 2^(n+2) - 3*n - 4.
 * https://oeis.org/A095264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+2) - 3*n - 4
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+2)   (works for arbitrary-precision types) */
    seq_t pow2 = 1;
    idx_t times = n + 2;
    for (idx_t i = 0; i < times; ++i)
        pow2 *= 2;      /* multiply by 2; defined for unsigned types */

    /* compute 2^(n+2) - 3*n - 4 */
    seq_t three_n = (seq_t)3 * (seq_t)n;
    return pow2 - three_n - 4;
}
