/*
 * A050720  Number of nonzero palindromes of length n containing the digit
 *          '0'.
 * https://oeis.org/A050720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*10^(ceil(n/2)-1) - 9^ceil(n/2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ceil(n/2) = (n+1)/2 for integer n >= 1 */
    idx_t k = (n + 1) >> 1;          /* (n+1)/2 */
    seq_t pow10 = 1;
    for (idx_t i = 1; i < k; ++i)    /* compute 10^(k-1) */
        pow10 *= 10;
    seq_t pow9 = 1;
    for (idx_t i = 1; i <= k; ++i)   /* compute 9^k */
        pow9 *= 9;
    return (9 * pow10) - pow9;
}
