/*
 * A080924  Jacobsthal gap sequence.
 * https://oeis.org/A080924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n)=4^n-1, a(2n+1)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 1 for all odd indices. */
    if (n & 1ULL)
        return (seq_t)1;

    /* For even indices compute 4^(n/2)-1.  Use exponentiation by
       squaring to keep the number of multiplications logarithmic. */
    idx_t k = n >> 1;          /* k = n/2 */
    seq_t result = (seq_t)1;   /* 4^0 = 1 */
    seq_t base   = (seq_t)4;   /* base = 4 */

    while (k > 0)
    {
        if (k & 1ULL)          /* if the current bit of k is set */
            result *= base;    /* multiply by the current power of 4   */
        base *= base;          /* advance to the next power of 4       */
        k >>= 1;               /* shift k right to process next bit    */
    }

    /* Subtract 1 to get 4^k - 1. */
    result -= (seq_t)1;
    return result;
}
