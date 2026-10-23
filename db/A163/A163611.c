/*
 * A163611  a(n) = ((5 + 2*sqrt(2))*(5 + sqrt(2))^n + (5 - 2*sqrt(2))*(5 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((5 + 2*sqrt(2))*(5 + sqrt(2))^n + (5 - 2*sqrt(2))*(5 -
 *            sqrt(2))^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 5, a(1) = 29, a(n) = 10*a(n-1) - 23*a(n-2) for n > 1 */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0u) return (seq_t)5;
    if (n == 1u) return (seq_t)29;

    /* iterative evaluation of the linear recurrence */
    seq_t a0 = (seq_t)5;      /* a(0) */
    seq_t a1 = (seq_t)29;     /* a(1) */
    seq_t an = a0;            /* placeholder for a(n) */

    for (idx_t i = 2u; i <= n; ++i)
    {
        an = (seq_t)(10 * a1 - 23 * a0);
        a0 = a1;
        a1 = an;
    }
    return an;
}
