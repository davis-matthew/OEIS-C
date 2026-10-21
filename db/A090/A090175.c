/*
 * A090175  a(n) = Mod[n,10+Sum[Mod[Floor[n/10^m],10],{m,1,10}]].
 * https://oeis.org/A090175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Mod[n, 10 + Sum[Mod[Floor[n/10^m],10],{m,1,10}]]
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t temp = (seq_t)n / 10;          /* val of floor(n/10)          */
    for (int i = 0; i < 10; ++i)        /* add digits from 2nd to 11th */
    {
        sum += temp % 10;
        temp /= 10;
    }
    seq_t den = 10 + sum;               /* denominator in the modulo    */
    seq_t sn  = (seq_t)n;               /* cast index to seq_t for mod */
    return sn % den;
}
