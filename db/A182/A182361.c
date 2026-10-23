/*
 * A182361  a(n+1) = a(n) + floor(a(n)/8) with a(0)=8.
 * https://oeis.org/A182361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 8; a(n+1) = a(n) + floor(a(n)/8) */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)8;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t inc = val / (seq_t)8;
        val += inc;
    }
    return val;
}
