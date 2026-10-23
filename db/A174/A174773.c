/*
 * A174773  y-values in the solution to x^2 - 34*y^2 = 1.
 * https://oeis.org/A174773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A174773
   Recurrence: a(1) = 0, a(2) = 6,
                a(n) = 70*a(n-1) - a(n-2) for n >= 3
*/
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;

    seq_t a1 = (seq_t)0;      /* a(1) */
    seq_t a2 = (seq_t)6;      /* a(2) */
    seq_t a  = a2;            /* placeholder for current term */
    for (idx_t i = 3; i <= n; ++i)
    {
        a = (seq_t)70 * a2 - a1;
        a1 = a2;
        a2 = a;
    }
    return a2;
}
