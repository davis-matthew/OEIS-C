/*
 * A090616  Exponent of highest power of 4 dividing n!.
 * https://oeis.org/A090616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n - popcount(n))/2), where popcount(n) is the
 *            number of 1‑bits in the binary representation of n. This follows
 *            from Legendre’s formula for v₂(n!) = n - popcount(n) and the
 *            relation v₄ = ⌊v₂/2⌋
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t popcount_u64(idx_t x)
{
    idx_t c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* v₂(n!) = n - popcount(n) */
    seq_t v2 = (seq_t)n - (seq_t)popcount_u64(n);
    /* exponent of 4 in n! */
    return v2 >> 1;
}
