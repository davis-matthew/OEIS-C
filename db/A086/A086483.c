/*
 * A086483  Bit that is two places to left of least significant 1-bit in the
 *          binary expansion of n.
 * https://oeis.org/A086483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the value of the bit two positions left of the least
 *            significant 1‑bit in the binary expansion of n; a(0) = 0. The
 *            bit is obtained by shifting n right by (position_of_lsb(n) + 2)
 *            and masking with 1
 * Range:     u8 n <= 2495, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the position of the least significant set bit (LSB). */
    idx_t tmp = n;
    idx_t tz  = 0;            /* number of trailing zeros before first 1 */
    while ((tmp & 1) == 0) {
        tz++;
        tmp >>= 1;
    }

    /* Shift original n by (tz + 2) and return the LSB of the result. */
    return ((seq_t)n >> (tz + 2)) & (seq_t)1;
}
