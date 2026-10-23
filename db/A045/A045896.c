/*
 * A045896  Denominator of n/((n+1)*(n+2)) = A026741/A045896.
 * https://oeis.org/A045896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   a(n) = (n+1)*(n+2) when n is odd; a(n) = (n+1)*(n+2)/2 when n is
 *            even
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t prod = (nseq + 1) * (nseq + 2);
    if ((n & 1) == 0)      /* even n */
        return prod / 2;   /* division is exact */
    else                   /* odd n */
        return prod;
}
