/*
 * A160397  Numerator of Hermite(n, 15/32).
 * https://oeis.org/A160397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15 * a(n-1) - 512 * (n-1) * a(n-2) with a(0) = 1, a(1) =
 *            15. This recurrence follows from the Hermite polynomial
 *            recurrence H_{n+1}(x) = 2x H_n(x) - 2n H_{n-1}(x) with x = 15/32
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;

    seq_t p_prevprev = (seq_t)1;   /* a(0) */
    seq_t p_prev     = (seq_t)15;  /* a(1) */
    seq_t p_next;

    for (idx_t i = 1; i < n; ++i)  /* i = n-1 corresponds to a(i+1) */
    {
        /* p_next = 15 * p_prev - 512 * i * p_prevprev */
        seq_t coeff = (seq_t)512 * (seq_t)i;  /* 512 * (n-1) */
        p_next = (seq_t)15 * p_prev - coeff * p_prevprev;

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_next;
    }
    return p_prev;
}
