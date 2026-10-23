/*
 * A047264  Numbers that are congruent to 0 or 5 mod 6.
 * https://oeis.org/A047264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + (-1)^n - 2
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nSeq = (seq_t)n;
    seq_t res  = 3 * nSeq;
    if (n & 1)        /* n is odd: (-1)^n = -1 */
        res -= 3;     /* 3*n - 3  */
    else              /* n is even: (-1)^n =  1 */
        res -= 1;     /* 3*n - 1  */
    return res;
}
