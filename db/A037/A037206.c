/*
 * A037206  Duplicate of A065530.
 * https://oeis.org/A037206
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(n-2) for even n, a(n) = n-2 for odd n (n ≥ 2)
 * Range:     i8 n <= 13, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 2 by the problem statement; behaviour for smaller n is irrelevant */
    seq_t val = (seq_t)n - 2;
    if ((n & 1) == 0)      /* n is even */
        val *= (seq_t)n;   /* multiply by n */
    return val;
}
