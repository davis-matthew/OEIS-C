/*
 * A059893  Reverse the order of all but the most significant bit in binary
 *          expansion of n: if n = 1ab..yz then a(n) = 1zy..ba.
 * https://oeis.org/A059893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A059893: a(n) = reverse all but the most significant bit of the binary
 * expansion of n.  Implemented directly without external helpers.
 *
 * For n ≥ 1:
 *   Let m = ⌊log2(n)⌋ (position of the highest set bit)  
 *   Let prefix = 1 << m (the fixed leading 1)  
 *   Let r = n - prefix (remaining lower bits)  
 *   Reverse the m lower bits of r and add to prefix.
 *
 * This algorithm runs in O(log n) time and uses only integer arithmetic.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined for n=0, return 0 */

    /* Find m = floor(log2(n)) */
    unsigned int m = 0;
    idx_t tmp = n;
    while ((tmp >>= 1) != 0)
        ++m;

    idx_t prefix = (idx_t)1 << m;             /* leading 1 bit */
    idx_t r = n - prefix;                     /* lower bits */

    /* Reverse the lower m bits of r */
    seq_t rev = (seq_t)0;
    for (unsigned int i = 0; i < m; ++i)
    {
        if (r & ((idx_t)1 << i))
            rev |= (seq_t)1 << (m - 1 - i);
    }

    return (seq_t)(prefix | rev);
}
