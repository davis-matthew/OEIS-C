/*
 * A227805  Sum of even numbers starting at 2, alternating signs.
 * https://oeis.org/A227805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = n + 4 if n is even, 3 - n if n is odd
 * Range:     i8 n <= 123, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A227805: Sum of even numbers starting at 2 with alternating signs */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Check whether n is odd or even using bitwise AND */
    if (n & (idx_t)1u) {
        /* n is odd: a(n) = 3 - n */
        return (seq_t)3 - N;
    } else {
        /* n is even: a(n) = n + 4 */
        return N + (seq_t)4;
    }
}
