/*
 * A015578  Expansion of x/(1 - 8*x - 11*x^2).
 * https://oeis.org/A015578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 11*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iteratively compute the recurrence while keeping only the
       previous two values.  This works for any seq_t type that
       supports the required arithmetic operations. */
    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t an = a1;        /* current term */

    for (idx_t i = 2; i <= n; ++i) {
        an = 8 * a1 + 11 * a0;  /* a(i) = 8*a(i-1) + 11*a(i-2) */
        a0 = a1;                /* shift window */
        a1 = an;
    }
    return an;
}
