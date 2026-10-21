/*
 * A071998  Write n in binary, interpret that as a decimal number, convert
 *          back to binary.
 * https://oeis.org/A071998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A007088(n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 95, u128 n <= 4095,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t d1 = 0;
    /* First conversion: binary representation of n -> decimal d1 */
    if (n != 0) {
        idx_t len1 = 0;
        idx_t t = n;
        while (t) {
            ++len1;
            t >>= 1;
        }
        for (idx_t i = len1; i--; ) {
            seq_t bit = (seq_t)((n >> (unsigned long long)i) & 1);
            d1 = d1 * (seq_t)10 + bit;
        }
    }

    /* Second conversion: binary representation of d1 -> decimal d2 */
    seq_t d2 = 0;
    if (d1 != 0) {
        idx_t len2 = 0;
        seq_t t2 = d1;
        while (t2) {
            ++len2;
            t2 >>= 1;
        }
        for (idx_t i = len2; i--; ) {
            seq_t bit = (seq_t)((d1 >> (unsigned long long)i) & 1);
            d2 = d2 * (seq_t)10 + bit;
        }
    }

    return d2;
}
