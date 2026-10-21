/*
 * A079863  a(n) is the number of occurrences of 11s in the palindromic
 *          compositions of m=2*n-1 = the number of occurrences of 12s in the
 *          palindromic compositions of m=2*n.
 * https://oeis.org/A079863
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 22) * 2^(n-12) (valid for n >= 12)
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 69, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 12)
        return 0;               /* undefined for n < 12 – value unused */

    seq_t result = (seq_t)(n + 22);  /* (n+22) */
    idx_t shift = n - 12;           /* exponent of 2 */

    /* multiply by 2^shift by repeated left shift of 1; safe for all unsigned types */
    for (idx_t i = 0; i < shift; ++i)
        result <<= 1;

    return result;
}
