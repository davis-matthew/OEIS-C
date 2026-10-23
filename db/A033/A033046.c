/*
 * A033046  Sums of distinct powers of 9.
 * https://oeis.org/A033046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i≥0} bit_i(n) * 9^i, where bit_i(n) is the i-th binary
 *            digit of n (0 or 1)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t pow = (seq_t)1;      /* 9^0 */
    while (n)
    {
        if (n & 1)           /* when the current bit is 1 */
            sum += pow;      /* add the corresponding power of 9 */
        pow *= (seq_t)9;     /* move to the next power of 9 */
        n >>= 1;             /* shift to process the next bit */
    }
    return sum;
}
