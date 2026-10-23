/*
 * A143812  Maximal number of halving and tripling steps to reach 1 in '3x+1'
 *          problem for range (1, ..., n).
 * https://oeis.org/A143812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum number of terms (including both endpoints) in the
 *            Collatz sequence 3x+1 / 2 for all integers 1..n
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t best = 1;               /* at least the sequence 1,1 has length 1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t x   = (seq_t)i;     /* start value of the Collatz sequence   */
        seq_t len = 1;            /* count the starting term itself       */

        while (x != 1)            /* evolve until the sequence reaches 1  */
        {
            if ((x & 1) == 0)     /* even: halve                                 */
                x = x >> 1;
            else                  /* odd: 3x+1                                  */
                x = 3 * x + 1;
            ++len;
        }
        if (len > best)          /* keep the maximum observed length          */
            best = len;
    }
    return best;
}
