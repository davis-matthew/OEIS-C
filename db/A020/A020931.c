/*
 * A020931  Expansion of (1-4*x)^(19/2).
 * https://oeis.org/A020931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-4)^n * binomial(19/2, n) Recurrence: a(0) = 1 a(n) =
 *            a(n‑1) * (4*n - 42) / n for n >= 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 27, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;          /* a(0) */

    /* Iterate up to n using the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        /* factor numerator and integer divisor */
        seq_t numerator   = (seq_t)4 * (seq_t)i - (seq_t)42;
        seq_t multiplier  = result * numerator;   /* multiplication first  */
        result            = multiplier / (seq_t)i; /* exact division next   */
    }

    return result;
}
