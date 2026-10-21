/*
 * A097257  Numbers whose set of base 11 digits is {0,A}, where A base 11 = 10
 *          base 10.
 * https://oeis.org/A097257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10 * Σ_{i: i‑th bit of n is 1} 11^i This follows from
 *            a(2n) = 11·a(n) and a(2n+1) = a(2n)+10, with a(0) = 0. The
 *            computation iterates over the bits of n, adding the
 *            corresponding power of 11 to a running sum, then multiplies the
 *            result by 10
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power   = 1;           /* 11^0 */

    while (n != 0) {
        if (n & 1) {
            result += power;    /* add 11^i when i-th bit is 1 */
        }
        power *= 11;            /* advance to next power of 11 */
        n >>= 1;                /* move to next bit */
    }
    return result * 10;          /* multiply by 10 to get digits {0,A} */
}
