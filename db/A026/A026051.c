/*
 * A026051  a(n) = (d(n)-r(n))/5, where d = A026049 and r is the periodic
 *          sequence with fundamental period (4,1,4,0,1).
 * https://oeis.org/A026051
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5) + n^2 - 10*n + 39 for n >= 12 with initial values
 *            a(7)=23, a(8)=31, a(9)=40, a(10)=53, a(11)=68
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the base values directly for the first equivalence classes. */
    if (n <= 11)
    {
        static const seq_t base[5] = {23, 31, 40, 53, 68};
        return base[n - 7];                /* n in [7,11] */
    }

    /* Recursive step – use the closed recurrence derived from the
       given formula.  The recursion depth is about (n-7)/5 ≤ 2 000
       for n ≤ 10 000, which is well within limits. */
    seq_t inc = (seq_t)n * (seq_t)n          /* n^2          */
              - (seq_t)10 * (seq_t)n          /* -10*n        */
              + (seq_t)39;                   /* +39          */
    return a(n - 5) + inc;
}
