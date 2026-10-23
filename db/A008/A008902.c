/*
 * A008902  x->x/2 if x even, x->3x-1 if x odd.
 * https://oeis.org/A008902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=99; for n≥0, a(n+1)=a(n)/2 if a(n) even, otherwise 3*a(n)-1.
   The sequence is the orbit of 99 under the rule x→x/2 (x even), x→3x-1 (x odd). */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)99;               /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        if ((val % (seq_t)2) == 0)
            val /= 2;                   /* even case */
        else
            val = val * 3 - 1;          /* odd case */
    }
    return val;                          /* a(n) */
}
