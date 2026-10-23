/*
 * A163471  a(n) = 10*a(n-1) - 22*a(n-2) for n > 1; a(0) = 3, a(1) = 18.
 * https://oeis.org/A163471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) – 22*a(n-2), with a(0) = 3, a(1) = 18
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0u)
        return (seq_t)3;
    if (n == 1u)
        return (seq_t)18;

    /* Recurrence: a[n] = 10*a[n-1] - 22*a[n-2] */
    seq_t a0 = (seq_t)3;   /* a(0) */
    seq_t a1 = (seq_t)18;  /* a(1) */
    seq_t an;

    for (idx_t i = 2u; i <= n; ++i)
    {
        an = (seq_t)10 * a1 - (seq_t)22 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
