/*
 * A059448  The parity of the number of zero digits when n is written in
 *          binary.
 * https://oeis.org/A059448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = parity(floor_log2(n)+1) XOR parity(popcount(n)) where
 *            parity(x) is 0 if x is even, 1 if x is odd. This equals the
 *            parity of the number of zero bits in the binary representation
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute parity of the number of 1-bits (popcount) */
    idx_t ones_parity = 0;
    idx_t t = n;
    while (t) {
        if (t & 1) ones_parity ^= 1;
        t >>= 1;
    }

    /* compute parity of the length of the binary representation */
    idx_t len_parity = 0;
    t = n;
    while (t) {
        len_parity ^= 1;
        t >>= 1;
    }

    /* result is XOR of the two parities */
    seq_t res = (seq_t)(ones_parity ^ len_parity);
    return res;
}
