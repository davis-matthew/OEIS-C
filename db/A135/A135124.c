/*
 * A135124  Numbers such that the digital sums in base 2, base 4 and base 8
 *          are all equal.
 * https://oeis.org/A135124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k>=0} ((n >> k) & 1) * 64^k (interpreting the binary
 *            digits of n as base‑64 digits)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 2047,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;   /* accumulated value */
    seq_t power  = 1;   /* 64^k, starting with k = 0   */
    idx_t x = n;        /* work on a copy of the index */

    while (x > 0)
    {
        if (x & 1)          /* if the current binary digit is 1,
                               add the corresponding power of 64   */
            result += power;

        power *= 64;        /* move to the next 64 power (k++)   */
        x >>= 1;            /* advance to the next binary digit   */
    }

    return result;
}
