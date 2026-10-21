/*
 * A216195  Abelian complexity function of the period-doubling sequence
 *          (A096268).
 * https://oeis.org/A216195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2 a(2n) = a(n) a(4n-1)= a(n)+1 a(4n+1)= a(n)+1
 *            (equivalently: a(n) = 2 + number of odd reductions applied while
 *            repeatedly applying the above rules until n becomes 1.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 2;
    while (n > 1)
    {
        if ((n & 1) == 0)          /* even: a(2n)=a(n)     */
            n >>= 1;
        else                       /* odd */
        {
            if ((n & 3) == 1)      /* n = 4k+1  ->  n = (n-1)/4  */
                n = (n - 1) >> 2;
            else                   /* n = 4k+3  ->  n = (n+1)/4  */
                n = ((n - 1) >> 2) + 1;  /* avoid overflow of n+1  */
            ++res;                 /* each odd step adds 1   */
        }
    }
    return res;
}
