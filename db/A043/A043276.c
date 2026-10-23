/*
 * A043276  a(n) = maximal run length in base-2 representation of n.
 * https://oeis.org/A043276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = maximal run length of consecutive identical bits in the
 *            binary representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 the maximal run length is at least 1 */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t last = n & (idx_t)1;   /* least significant bit */
    idx_t cur  = 1;
    idx_t max  = 1;

    for (idx_t m = n >> 1; m; m >>= 1) {
        idx_t bit = m & (idx_t)1;

        if (bit == last) {
            cur++;
        } else {
            if (cur > max) {
                max = cur;
            }
            cur  = 1;
            last = bit;
        }
    }
    if (cur > max) max = cur;

    return (seq_t)max;
}
