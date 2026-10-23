/*
 * A145694  Numbers Y such that 57*Y^2+19 is a square.
 * https://oeis.org/A145694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=5, a(2)=1515, a(n+2)=302*a(n+1)-a(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function computes the term a(n) of OEIS sequence A145694
 *  (Numbers Y such that 57*Y^2+19 is a square) using the linear
 *  recurrence.  It works for any idx_t ≥ 1 and any seq_t width
 *  (8, 16, 32, 64, 128 bits or an arbitrary‑precision class).
 *  No external utilities are required.                               */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) /* undefined for 0, but return 0 to avoid UB */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)5;
    if (n == 2)
        return (seq_t)1515;

    /* iterate the recurrence from 3 up to n */
    seq_t prevprev = (seq_t)5;      /* a(1) */
    seq_t prev     = (seq_t)1515;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev * (seq_t)302 - prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;   /* prev holds a(n) */
}
