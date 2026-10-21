/*
 * A077552  Consider the following triangle in which the n-th row contains n
 *          distinct numbers whose product is the smallest and has the least
 *          possible number of divisors. 1 is a member of only the first row.
 *          Sequence contains the final term of the rows (the leading
 *          diagonal).
 * https://oeis.org/A077552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=2^(n+1) for n>=2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t res = (seq_t)1;
    idx_t e = n + 1;          /* exponent of 2 */
    while (e-- > 0)
        res *= (seq_t)2;
    return res;
}
