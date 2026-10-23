/*
 * A100525  Bisection of A048654.
 * https://oeis.org/A100525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0) = 4, a(1) = 22
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)22;

    seq_t a0 = (seq_t)4;   /* a(0) */
    seq_t a1 = (seq_t)22;  /* a(1) */
    seq_t an = 0;          /* current value */

    for (idx_t i = 2; i <= n; ++i) {
        an = 6 * a1 - a0; /* recurrence */
        a0 = a1;
        a1 = an;
    }
    return an;
}
