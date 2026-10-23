/*
 * A131322  Row sums of triangle A131321.
 * https://oeis.org/A131322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - a(n-4), * a(0) = 1, a(1) =
 *            1, a(2) = 3, a(3) = 5 * (Form derived from the generating
 *            function (1-x²)/(1 - x - 3x² + x³ + x⁴))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* Use four sliding variables to generate the sequence up to n. */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)3;   /* a(n-2) */
    seq_t a3 = (seq_t)5;   /* a(n-1) */
    seq_t an = (seq_t)0;   /* a(n) to be computed */

    /* Iterate from 4 up to n inclusive. */
    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + (seq_t)3 * a2 - a1 - a0;  /* recurrence relation */
        a0 = a1;  /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
