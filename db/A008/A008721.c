/*
 * A008721  Molien series for 3-dimensional group [2,7] = *227.
 * https://oeis.org/A008721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{c=0..⌊n/7⌋, c≡n (mod2)} ((n-7c)/2 + 1)
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxc = n / 7;
    idx_t parity = n & 1;
    for (idx_t c = 0; c <= maxc; ++c)
    {
        if ((c & 1) != parity) continue;          /* parity check */
        idx_t rem = n - 7 * c;                    /* remaining sum after 7's */
        seq_t t = (seq_t)(rem / 2);               /* rest divided into twos */
        sum += t + 1;                             /* (a + b) = t, two kinds of 2's */
    }
    return sum;
}
