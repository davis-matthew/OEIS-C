/*
 * A179854  Number of 0's (mod 3) in the binary expansion of n.
 * https://oeis.org/A179854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# of 0 bits in binary representation of n) mod 3
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Count zeros in the binary expansion of n */
    idx_t zeros = 0;
    for (idx_t temp = n; temp; temp >>= 1) {
        if (!(temp & 1))
            zeros += 1;
    }
    return (seq_t)(zeros % 3);
}
