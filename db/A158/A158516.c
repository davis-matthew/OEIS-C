/*
 * A158516  a(n) = Hermite(n,6).
 * https://oeis.org/A158516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 12*a(n-1) - 2*(n-1)*a(n-2) with a(0) = 1, a(1) = 12
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 8, i64 n <= 18, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function respects the required types seq_t (value) and idx_t (index). */
seq_t a(idx_t n)
{
    /* Handle trivial cases first */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)12;

    /* Iterate using the recurrence without any external allocation.
       The variables a0 and a1 hold a(n-2) and a(n-1), respectively. */
    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)12;         /* a(1) */
    seq_t an = a1;                /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 12*a(i-1) - 2*(i-1)*a(i-2) */
        an = 12 * a1 - 2 * (seq_t)(i - 1) * a0;
        a0 = a1;
        a1 = an;
    }

    return an;
}
