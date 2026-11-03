/*
 * A117942  a(n) = a(3n) = -a(3n+1) = -a(3n+2)/2.
 * https://oeis.org/A117942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4372, i16 n <= 19683, i32 n <= 19683, i64 n <= 19683,
 *            i128 n <= 19683, big any n
 * Validated: standard, pass, 108 terms through n = 9028 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(3n) = a(n), a(3n+1) = -a(n), a(3n+2) = -2*a(n); a(0)=1 */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    idx_t m = n;
    while (m)
    {
        idx_t r = m % (idx_t)3;
        m   /= (idx_t)3;
        if (r == (idx_t)1)
            res = res * (seq_t)-1;
        else if (r == (idx_t)2)
            res = res * (seq_t)-2;
        /* r==0 multiplies by 1, no change */
    }
    return res;
}
