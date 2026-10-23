/*
 * A049501  Major index of n, first definition.
 * https://oeis.org/A049501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = sum of positions of '1' bits that are immediately
 *            followed by a '0' in the binary representation of n, counting
 *            the leftmost bit as position 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Determine number of bits (position of the most significant 1 plus 1) */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) msb++;
    msb++;  /* msb = total number of bits in n */

    seq_t sum = 0;
    /* Loop over all bit positions except the least significant bit,
       checking for transitions 1 -> 0 to the right. */
    for (idx_t i = msb - 1; i > 0; --i) {
        if (((n >> i) & 1ULL) && !((n >> (i - 1)) & 1ULL)) {
            sum += (seq_t)(msb - i);   /* position of the '1' bit */
        }
    }
    return sum;
}
