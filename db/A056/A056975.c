/*
 * A056975  Number of blocks of {0, 0, 1} in binary expansion of n.
 * https://oeis.org/A056975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of occurrences of the binary block 001 in the
 *            binary expansion of n (MSB to LSB)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)
        return 0;                 /* defined only for n≥1, but safe */

    /* Find number of bits in n. */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++bitlen;
    }

    if (bitlen < 3)
        return 0;                 /* too short to contain 001 */

    /* Store bits from most significant to least significant. */
    unsigned char bits[64];
    for (idx_t i = bitlen; i > 0; --i)
        bits[bitlen - i] = (n >> (i - 1)) & 1U;

    /* Count each 001 block. */
    for (idx_t i = 0; i + 2 < bitlen; ++i)
        if (bits[i] == 0 && bits[i + 1] == 0 && bits[i + 2] == 1)
            ++count;

    return count;
}
