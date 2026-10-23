/*
 * A131386  a(n) = (-1)^n*n*(n-2).
 * https://oeis.org/A131386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*n*(n-2)
 * Range:     i8 n <= 12, i16 n <= 182, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The input n is guaranteed to be >= 1 (OEIS offset = 1). */
    /* Compute the formula directly using seq_t arithmetic. */
    seq_t seqn = (seq_t)n;
    seq_t prod = seqn * (seqn - 2); /* n*(n-2) */
    if (n & 1U)                  /* if n is odd, multiply by -1 */
        return -prod;
    else
        return prod;             /* if n is even, result is positive */
}
