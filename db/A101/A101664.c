/*
 * A101664  Fixed point of morphism 0 -> 01, 1 -> 12, 2 -> 00.
 * https://oeis.org/A101664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = [a(n)==1], a(2n+1) = (a(n)+1) mod 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t val = (seq_t)0;

    /* Find position of the most significant bit of n */
    idx_t msb = 0;
    for (idx_t tmp = n; tmp > 1; ++msb, tmp >>= 1);

    /* Process bits from most significant to least significant */
    for (idx_t i = msb; ; --i)
    {
        if (((n >> i) & 1ULL) != 0)
            val = (val + 1) % 3;   /* a(2k+1) = (a(k)+1) mod 3 */
        else
            val = (val == 1) ? 1 : 0; /* a(2k)   = [a(k)==1] */

        if (i == 0)
            break;
    }
    return val;
}
