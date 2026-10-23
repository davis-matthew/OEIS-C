/*
 * A008879  3x+1 sequence starting at 87.
 * https://oeis.org/A008879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=87; for n>=1,
   a(n)=3*a(n-1)+1 if a(n-1) odd, else a(n-1)/2. */

seq_t a(idx_t n)
{
    seq_t x = 87;           /* start value a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        if (x & 1)          /* odd */
            x = 3 * x + 1;
        else                /* even */
            x = x >> 1;     /* equivalent to x / 2 */
    }
    return x;
}
