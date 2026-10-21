/*
 * A084473  Replace 0 with 0000 in binary representation of n.
 * https://oeis.org/A084473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = replace each 0 of binary(n) by 0000, then interpret as
 *            binary
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find floor(log2(n)) for n > 0 */
static idx_t msb_idx(idx_t n) {
    idx_t pos = 0;
    while (n >>= 1)
        pos++;
    return pos;
}

seq_t a(idx_t n) {
    /* Sequence is defined for n >= 1 (offset 1) */
    if (n == 0) return 0;   /* safety, though not used */

    seq_t result = 0;
    idx_t msb_pos = msb_idx(n);          /* number of bits in n minus 1 */
    for (idx_t i = msb_pos + 1; i > 0; --i) {
        if ((n >> (i - 1)) & 1) {
            result = (result << 1) | 1;  /* copy a '1' bit */
        } else {
            result <<= 4;                /* copy four '0' bits */
        }
    }
    return result;
}
