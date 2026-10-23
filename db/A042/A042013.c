/*
 * A042013  Denominators of continued fraction convergents to sqrt(530).
 * https://oeis.org/A042013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 46*a(n-1) + a(n-2), a(0) = 1, a(1) = 46
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)46;

    /* Iterate the linear recurrence in a simple loop. */
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)46;     /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t tmp = a1 * (seq_t)46 + a0;   /* a(i) = 46*a(i-1) + a(i-2) */
        a0 = a1;
        a1 = tmp;
    }
    return a1;               /* a(n) is now held in a1 */
}
