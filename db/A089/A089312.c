/*
 * A089312  Write n in binary; a(n) = number represented by rightmost block of
 *          1's.
 * https://oeis.org/A089312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = value of the rightmost consecutive block of 1's in the binary representation of n */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* find the first (least significant) set bit */
    idx_t i = 0;
    while (!(n & ((idx_t)1 << i)))
        ++i;

    /* count how many consecutive 1's starting from that bit */
    idx_t j = i;
    while (j < 64 && (n & ((idx_t)1 << j)))
        ++j;

    idx_t blocklen = j - i;          /* 1 <= blocklen <= 64 */

    /* the block consists of blocklen ones : value = 2^blocklen - 1 */
    if (blocklen == 64)
        return (seq_t)(~(seq_t)0);   /* all 64 bits set */
    else
        return (seq_t)((1ULL << blocklen) - 1);
}
