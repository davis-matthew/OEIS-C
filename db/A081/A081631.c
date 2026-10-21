/*
 * A081631  a(n) = 2*2^n - (-2)^n.
 * https://oeis.org/A081631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*2^n - (-2)^n (i.e., a(n) = 2^n if n even, 3*2^n if n
 *            odd)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;          /* compute 2^n */
    if ((n & 1) == 0)      /* n even */
        return pow2;       /* a(n) = 2^n */
    else
        return pow2 * 3;   /* a(n) = 3*2^n for odd n */
}
