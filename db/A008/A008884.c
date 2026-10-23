/*
 * A008884  3x+1 sequence starting at 27.
 * https://oeis.org/A008884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 27, a(n) = 3*a(n-1)+1 if a(n-1) is odd, a(n-1)/2 if
 *            a(n-1) is even
 * Range:     u8 n <= 11, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)27;        /* a(0) */
    if (n == 0) return cur;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* If cur is odd: cur = 3*cur + 1; else: cur = cur / 2 */
        if (cur & (seq_t)1) 
            cur = cur * (seq_t)3 + (seq_t)1;
        else
            cur = cur >> 1;
    }
    return cur;
}
