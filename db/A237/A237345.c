/*
 * A237345  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(8).
 * https://oeis.org/A237345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0, let v=a(n-1)
   if v%10==8 then a(n) = (v/10)*100 + 64
   else          a(n) = v + 1 */
seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        if (val % (seq_t)10 == (seq_t)8)
            val = (val / (seq_t)10) * (seq_t)100 + (seq_t)64;
        else
            ++val;
    }
    return val;
}
