/*
 * A010146  Continued fraction for sqrt(62).
 * https://oeis.org/A010146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 9832, u16 n <= 9832, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic: a(0)=7; for n>=1, a(n)=1 if (n-1)%4==0 or 2,
   6 if (n-1)%4==1, and 14 if (n-1)%4==3. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    switch ((n - 1) & 3)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)6;
        case 2: return (seq_t)1;
        case 3: return (seq_t)14;
    }
    /* unreachable */
    return (seq_t)0;
}
