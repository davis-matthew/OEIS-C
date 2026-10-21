/*
 * A224508  a(n+2) = a(n+1) + a(n) + A*t^n, with A = 1 and t = -2.
 * https://oeis.org/A224508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+2) = a(n+1) + a(n) + (-2)^n; a(0) = 0, a(1) = 1
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 65, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iteratively compute up to a(n). */
    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t pow = (seq_t)1; /* (-2)^0 */

    /* We need to perform the recurrence for k = 0 to n-2. */
    idx_t limit = n - 2;
    for (idx_t k = 0; k <= limit; ++k)
    {
        /* a(k+2) = a(k+1) + a(k) + (-2)^k. */
        seq_t a2 = a1 + a0 + pow;

        /* Update (-2)^(k+1) for the next iteration. */
        pow *= (seq_t)(-2);

        /* Slide window for next step. */
        a0 = a1;
        a1 = a2;
    }

    /* After the loop, a1 holds a(n). */
    return a1;
}
