/*
 * A090702  a(n) is the minimal number k such that every binary word of length
 *          n can be transformed into a palindrome or an antipalindrome by
 *          deleting at most k letters.
 * https://oeis.org/A090702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 0 for n = 1,2 a(n) = floor( (n + 2* floor((n-3)/7) ) / 3
 *            ) for n >= 3
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2u)   /* n is unsigned by definition of idx_t   */
        return (seq_t)0;
    idx_t q = (n - 3u) / 7u;          /* floor((n-3)/7) */
    seq_t val = (seq_t)n;             /* cast index to seq_t   */
    val += (seq_t)(2u * q);           /* add 2 * floor((n-3)/7) */
    return val / (seq_t)3u;           /* integer division      */
}
