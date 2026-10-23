/*
 * A160065  Numerator of Hermite(n, 21/25).
 * https://oeis.org/A160065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 21/25)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 42 */
/*             a(n+1) = 42 * a(n) - 1250 * n * a(n-1)  for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)42;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)42;  /* a(1) */

    for (idx_t i = 1; i < n; ++i) {
        /* a_{i+1} = 42*a_i - 1250*i*a_{i-1} */
        seq_t next = (seq_t)42 * a1 - (seq_t)1250 * (seq_t)i * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
