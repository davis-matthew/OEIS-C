/*
 * A155456  Write (1+1/x)*log(1+x) = Sum c(n)*x^n; then a(n) = (n+1)!*c(n).
 * https://oeis.org/A155456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = -1, for n ≥ 2: a(n) = -(n-1) * a(n-1)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive implementation using factorial with alternating sign. */

/* seq_t and idx_t are defined in the build environment. */
seq_t a(idx_t n)
{
    /* Handle the first two special cases */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)-1;

    /* Compute the recurrence a(n) = -(n-1) * a(n-1) iteratively. */
    seq_t val = (seq_t)-1;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* Multiply by (i-1) and flip the sign. */
        val = -( (seq_t)(i - 1) * val );
    }
    return val;
}
