/*
 * A227363  a(n) = n + (n-1)*(n-2) + (n-3)*(n-4)*(n-5) +
 *          (n-6)*(n-7)*(n-8)*(n-9) + ... + ...*(n-n).
 * https://oeis.org/A227363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 45, u64 n <= 110, u128 n <= 281,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227363:
   a(n) = n + (n-1)*(n-2) + (n-3)*(n-4)*(n-5) + (n-6)*(n-7)*(n-8)*(n-9) + …
   It is the sum of products of consecutive descending blocks of lengths
   1, 2, 3, … starting at n and decreasing.
   The algorithm iteratively forms each block, multiplies its factors,
   adds the result to the running total, and then skips past the block
   to the next starting position.   */
seq_t a(idx_t n)
{
    seq_t sum = 0;           /* accumulated value of the sequence   */
    idx_t cur = n;           /* current starting value for a block  */
    for (idx_t len = 1; cur >= len - 1; ++len)
    {
        seq_t prod = 1;
        for (idx_t j = 0; j < len; ++j)
        {
            prod *= (seq_t)(cur - j);   /* multiply each factor of the block */
        }
        sum += prod;                      /* add block product to total      */
        if (cur == len - 1)               /* next subtraction would underflow */
            break;                          /* terminate after this last block  */
        cur -= len;                      /* advance to the start of the next block */
    }
    return sum;
}
