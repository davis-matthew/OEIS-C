/*
 * A135686  a(n) = a(n-1) + A000045(n)*a(n-2), a(1) = 1, a(2) = 1.
 * https://oeis.org/A135686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = a(n-1) + F(n) * a(n-2), a(1)=1, a(2)=1 (F(n) = Fibonacci
 *            number, F(1)=F(2)=1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t aPrevPrev = (seq_t)1;  /* a(1) */
    seq_t aPrev     = (seq_t)1;  /* a(2) */

    seq_t fibPrevPrev = (seq_t)1; /* F(1) */
    seq_t fibPrev     = (seq_t)1; /* F(2) */
    seq_t fibCurr, aCurr;

    for (idx_t k = 3; k <= n; ++k) {
        fibCurr = fibPrevPrev + fibPrev;        /* F(k)   */
        aCurr   = aPrev + fibCurr * aPrevPrev; /* a(k)   */

        /* shift variables for next iteration */
        fibPrevPrev = fibPrev;
        fibPrev     = fibCurr;

        aPrevPrev = aPrev;
        aPrev     = aCurr;
    }
    return aPrev;   /* a(n) */
}
