/*
 * A087069  a(n) = Sum_{k >= 0} floor(n/(4^k)).
 * https://oeis.org/A087069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} floor(n / 4^k)
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t div = 1;
    while (div <= n)
    {
        sum += (seq_t)(n / div);
        /* avoid overflow when shifting div left by 2 */
        if (div > n / 4)
            break;
        div <<= 2;                /* div *= 4 */
    }
    return sum;
}
