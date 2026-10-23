/*
 * A041023  Denominators of continued fraction convergents to sqrt(15).
 * https://oeis.org/A041023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041023: Denominators of continued fraction convergents to sqrt(15).
   Recurrence with offset 0:
        a(0) = 1
        a(1) = 1
        for n ≥ 2:
            if (n is even)  a(n) = 6*a(n-1) + a(n-2)
            else            a(n) =     a(n-1) + a(n-2)
*/

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)1;  /* a(1) */
    seq_t cur;
    idx_t i;

    for (i = 2; i <= n; ++i)
    {
        if (i & 1)           /* odd index */
            cur = prev + prevprev;
        else                 /* even index */
            cur = (seq_t)6 * prev + prevprev;
        prevprev = prev;
        prev     = cur;
    }
    return prev;
}
