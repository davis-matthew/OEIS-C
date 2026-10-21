/*
 * A059164  A hierarchical sequence (W'3{2,2}*cc - see A059126).
 * https://oeis.org/A059164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16 * 2^{v2(n+1)} (v2(k)=exponent of 2 dividing k)
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;        /* k = n+1 */
    idx_t v2 = 0;
    while ((m & 1) == 0) {  /* count trailing zeros of (n+1) */
        v2++;
        m >>= 1;
    }
    return ((seq_t)16) << (unsigned int)v2;
}
