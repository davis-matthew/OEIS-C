/*
 * A041145  Denominators of continued fraction convergents to sqrt(82).
 * https://oeis.org/A041145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 18*a(n-1) + a(n-2); a(0)=1, a(1)=18
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                return (seq_t)1;
    if (n == 1)                return (seq_t)18;

    seq_t prevprev = (seq_t)1; /* a(0) */
    seq_t prev     = (seq_t)18;/* a(1) */
    seq_t cur      = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)18 * prev + prevprev; /* a(i) */
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
