/*
 * A040187  Continued fraction for sqrt(202).
 * https://oeis.org/A040187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=14, a(1)=4, a(2)=1, a(3)=2, a(4)=2, a(5)=1, a(6)=4,
   a(7)=28, a(n)=a(n-7) for n>=8 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)14;
    switch (n % 7)
    {
        case 0: return (seq_t)28;   /* n % 7 == 0 and n > 0 */
        case 1: return (seq_t)4;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)2;
        case 5: return (seq_t)1;
        case 6: return (seq_t)4;
    }
    /* Control should never reach here */
    return (seq_t)0;
}
