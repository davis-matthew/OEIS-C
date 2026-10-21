/*
 * A079547  a(n) = ((n^6 - (n-1)^6) - (n^2 - (n-1)^2))/60.
 * https://oeis.org/A079547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1}^{n-1} (i^2 + i^4)/2 (a(1)=0)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 134, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t i2 = (seq_t)i * (seq_t)i;        /* i^2 */
        seq_t i4 = i2 * i2;                    /* i^4 */
        sum += (i2 + i4) / 2;                  /* (i^2 + i^4)/2 */
    }
    return sum;
}
