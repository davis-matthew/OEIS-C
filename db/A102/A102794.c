/*
 * A102794  Duplicate of A075415.
 * https://oeis.org/A102794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = concatenation of (n‑1) copies of digit '4', followed by
 *            '3', followed by (n‑1) copies of digit '5', followed by '6' (in
 *            decimal)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* prepend (n‑1) '4's */
    for (idx_t i = 0; i < n - 1; ++i)
        result = result * 10 + 4;

    /* add the central '3' */
    result = result * 10 + 3;

    /* append (n‑1) '5's */
    for (idx_t i = 0; i < n - 1; ++i)
        result = result * 10 + 5;

    /* final trailing '6' */
    result = result * 10 + 6;

    return result;
}
