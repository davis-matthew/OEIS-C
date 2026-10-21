/*
 * A059157  A hierarchical sequence (W'3{2,2}cc - see A059126).
 * https://oeis.org/A059157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24 * 16^{A007814(n)} where A007814(n) = exponent of 2 in
 *            (n+1)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = 24 * 16^{v₂(n+1)} */

seq_t a(idx_t n)
{
    /* Find v₂(n+1) – the number of trailing zeros in (n+1) */
    idx_t m = n + 1;
    idx_t v = 0;
    while ((m & 1) == 0)
    {
        ++v;
        m >>= 1;
    }

    /* Compute 24 * 16^v.  Multiplication by 16 is safe for any seq_t size. */
    seq_t result = 24;
    for (idx_t i = 0; i < v; ++i)
    {
        result *= 16;
    }
    return result;
}
