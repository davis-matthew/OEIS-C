/*
 * A059154  A hierarchical sequence (W'2{3}c - see A059126).
 * https://oeis.org/A059154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12 * 8^v2(n+1), where v2(k) is the exponent of 2 in k
 *            (i.e., the number of trailing zeros in the binary representation
 *            of k)
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute v2(n+1) */
    idx_t tz = 0;
    idx_t k  = n + 1;
    while ((k & 1) == 0) {
        tz++;
        k >>= 1;
    }

    /* compute 12 * 8^tz */
    seq_t result = (seq_t)12;
    while (tz--) {
        result *= (seq_t)8;
    }

    return result;
}
