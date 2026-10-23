/*
 * A170791  a(n) = n^9*(n^9 + 1)/2.
 * https://oeis.org/A170791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n^9 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t pow9 = 1;
    for (idx_t i = 0; i < 9; ++i)
        pow9 *= base;          /* compute n^9 */

    seq_t next = pow9 + 1;      /* n^9 + 1 */

    /* n^9 and n^9+1 are consecutive; one of them is even.
       Divide the even one by 2 first to avoid overflow.        */
    seq_t result;
    if ((pow9 & 1) == 0)        /* pow9 is even */
    {
        result = (pow9 >> 1) * next;
    }
    else                       /* next is even */
    {
        result = pow9 * (next >> 1);
    }

    return result;
}
