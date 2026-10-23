/*
 * A163807  Reverse the order of inner digits (all digits but the first and
 *          last) of n written in binary. a(n) = the decimal value of the
 *          result.
 * https://oeis.org/A163807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = n with the inner binary digits (all but the first and
 *            last) reversed
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast to seq_t for bit operations */

    /* Compute the number of bits in n */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* For numbers with at most two bits, nothing to reverse */
    if (len <= 2)
        return nn;

    /* Preserve the most significant and least significant bits. */
    seq_t msb_mask = ((seq_t)1) << (len - 1);
    seq_t msb = nn & msb_mask;
    seq_t lsb = nn & (seq_t)1;
    seq_t res = msb | lsb;

    /* Reverse the inner bits. */
    for (idx_t i = 1; i + 1 < len; ++i) {
        if ((nn >> i) & (seq_t)1) {
            res |= ((seq_t)1) << (len - 1 - i);
        }
    }

    return res;
}
