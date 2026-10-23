/*
 * A033048  Sums of distinct powers of 12.
 * https://oeis.org/A033048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ (bit_i(n) * 12^i)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    a(2n)   = 12 * a(n)         */
/*    a(2n+1) = a(2n) + 1         */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;          /* 12^0 */
    while (n)
    {
        if (n & 1)
            result += power;
        power *= 12;           /* move to next power of 12 */
        n >>= 1;              /* process next bit */
    }
    return result;
}
