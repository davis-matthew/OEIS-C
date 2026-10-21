/*
 * A235499  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(3).
 * https://oeis.org/A235499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0; a(n+1) = a(n)+1 unless a(n) ends with 3, in which case
 *            a(n+1) = (a(n)/10)*10 + 9
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        if (v % 10 == 3)
        {
            v = (v / 10) * 10 + 9;
        }
        else
        {
            ++v;
        }
    }
    return v;
}
