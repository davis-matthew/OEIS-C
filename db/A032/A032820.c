/*
 * A032820  Numbers whose set of base-8 digits is {1,4}.
 * https://oeis.org/A032820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4;
   for n>2: if n is odd  -> a(n)=8*a(floor(n/2))+1
   if n is even -> a(n)=8*a((n-1)/2)+4 */
seq_t a(idx_t n)
{
    if (n <= 2)
    {
        return (n == 1) ? ((seq_t)1) : ((seq_t)4);
    }
    if (n & 1U)                /* n odd */
    {
        return ((seq_t)8) * a(n >> 1U) + ((seq_t)1);
    }
    else                        /* n even */
    {
        return ((seq_t)8) * a(((n - 1U) >> 1U)) + ((seq_t)4);
    }
}
