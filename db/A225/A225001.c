/*
 * A225001  Duplicate of A047203.
 * https://oeis.org/A225001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n + floor((n-2)/4) for n >= 2, a(1) = 4. This generates
 *            the nth positive integer that is not congruent to 1 (mod 5),
 *            excluding 1 itself
 * Range:     i8 n <= 67, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;
    /* n >= 2, so (n-2) is non‑negative in unsigned arithmetic */
    return (seq_t)n + (seq_t)((n - 2) / 4);
}
