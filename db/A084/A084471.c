/*
 * A084471  Change 0 to 00 in binary representation of n.
 * https://oeis.org/A084471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(1) = 1, a(2k+1) = 2*a(k)+1, a(2k) = 4*a(k)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                       /* offset ≥1, but guard for safety */
    seq_t res = 0;
    /* locate most significant set bit of n */
    idx_t msb = 0;
    for (idx_t i = 63; i > 0; --i) {
        if (n >> i) {
            msb = i;
            break;
        }
    }
    /* process bits from most significant to least */
    for (idx_t i = msb;; --i) {
        int bit = (n >> i) & 1u;
        res = (res << 1) | (seq_t)bit;
        if (bit == 0)
            res = res << 1;              /* 0 becomes 00 */
        if (i == 0)
            break;
    }
    return res;
}
