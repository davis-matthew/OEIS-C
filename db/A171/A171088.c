/*
 * A171088  To find 3 consecutive integers in the sequence, you have to take 4
 *          consecutive terms, no more and no less.
 * https://oeis.org/A171088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = k+1, a(3k+1) = k+3, a(3k+2) = k (derived from
 *            a(n)=1+a(n-3) with a(0)=1, a(1)=3, a(2)=0)
 * Range:     u8 n <= 759, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    idx_t r = n % 3;
    if (r == 0)        return (seq_t)(k + 1);
    else if (r == 1)   return (seq_t)(k + 3);
    else               return (seq_t)(k);
}
