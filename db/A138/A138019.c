/*
 * A138019  Period 5: repeat [1, 1, 0, -1, -1].
 * https://oeis.org/A138019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = [1, 1, 0, -1, -1][n mod 5]
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A138019: a pattern of period 5 */
seq_t a(idx_t n)
{
    /* Static array holding the 5-period pattern, 1-indexed by mod 5 */
    static const seq_t pattern[5] = {1, 1, 0, -1, -1};

    return pattern[n % 5];
}
