/*
 * A050604  Column 3 of A050600: a(n) = add1c(n,3).
 * https://oeis.org/A050604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4n) = 1 a(4n+1) = b(n) + 2 a(4n+2) = b(n) + 1 a(4n+3) = b(n) +
 *            1 where b(0) = 1 and for m>0: b(2k) = 1 b(2k+1) = b(k) + 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t b_value(idx_t n)
{
    /* b(0) = 1, and for all n>0:
       if n is even -> 1
       if n is odd  -> b((n-1)/2) + 1   */
    if ((n & 1) == 0)
        return (seq_t)1;
    return b_value((n - 1) >> 1) + 1;
}

seq_t a(idx_t n)
{
    switch (n & 3)          /* n modulo 4 */
    {
        case 0:             /* 4n */
            return (seq_t)1;
        case 1:             /* 4n+1 */
            return b_value(n >> 2) + 2;
        case 2:             /* 4n+2 */
        case 3:             /* 4n+3 */
            return b_value(n >> 2) + 1;
    }
    /* unreachable */
    return (seq_t)0;
}
