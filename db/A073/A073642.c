/*
 * A073642  Replace 2^k in the binary representation of n with k (i.e., if n =
 *          2^b + 2^c + 2^d + ... then a(n) = b + c + d + ...).
 * https://oeis.org/A073642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of the positions of the set bits in the binary
 *            representation of n (least significant bit has position 0).
 *            Equivalent to: a(n) = Σ_{k≥0} k * bit_k(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t pos = 0;
    /* Work with the index n as a bit mask */
    while (n != 0) {
        if (n & 1)
            res += (seq_t)pos;   /* add the bit position */
        n >>= 1;
        pos++;
    }
    return res;
}
