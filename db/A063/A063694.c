/*
 * A063694  Remove odd-positioned bits from the binary expansion of n.
 * https://oeis.org/A063694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n with all bits at odd positions cleared (i.e. keep only
 *            bits at indices 0, 2, 4, …)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t shift = 0;
    seq_t v = (seq_t)n;               /* work in seq_t type to allow big ints */
    while (v) {
        if (v & 1)                    /* if current LSB is 1, keep it in res */
            res |= ((seq_t)1 << shift);
        v >>= 2;                     /* skip the cleared odd bit */
        shift += 2;
    }
    return res;
}
