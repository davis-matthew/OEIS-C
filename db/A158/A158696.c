/*
 * A158696  a(n) = Hermite(n, 17).
 * https://oeis.org/A158696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 34*a(n-1) - 2*(n-1)*a(n-2), a(0)=1, a(1)=34
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 12, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)34;

    /* a(n) is computed iteratively from a(0) and a(1). */
    seq_t a0 = (seq_t)1;          /* a(n-2) */
    seq_t a1 = (seq_t)34;         /* a(n-1) */
    seq_t an;                     /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)34 * a1 - (seq_t)2 * ((seq_t)(i - 1)) * a0;
        a0 = a1;                  /* shift window: a(n-2) <- a(n-1) */
        a1 = an;                  /* a(n-1) <- a(n)   */
    }

    return a1; /* a1 holds a(n) after the loop */
}
