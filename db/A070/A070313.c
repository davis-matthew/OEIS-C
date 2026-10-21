/*
 * A070313  a(n) = 2^n - (2*n+1).
 * https://oeis.org/A070313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - (2*n+1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n by repeated multiplication.  Works for any seq_t
       that supports the * operator and the integer literal 2. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* Compute the subtrahend 2*n+1.  Cast the index to seq_t
       before arithmetic to avoid unintended integer promotion. */
    seq_t sub = (seq_t)n * 2 + 1;

    return pow2 - sub;
}
