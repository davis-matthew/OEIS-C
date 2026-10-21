/*
 * A075328  Difference between n-th pair in A075325.
 * https://oeis.org/A075328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*(n/2) + 2 when n is even, * a(n) = 5*((n+1)/2) when n
 *            is odd. * This matches the sequence A075328: 2,5,7,10,12,…
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1U) == 0U) /* even */
        return ((seq_t)(n >> 1U) * 5) + 2;
    else                /* odd  */
        return (((seq_t)(n + 1U) >> 1U) * 5);
}
