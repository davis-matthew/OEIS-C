/*
 * A097065  Interleave n+1 and n-1.
 * https://oeis.org/A097065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor((n+2)/2) - 2*(n mod 2)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n/2 in the unsigned idx_t type. */
    idx_t half = n / 2;

    /* Build the result using seq_t arithmetic. */
    seq_t result = (seq_t)(half + 1) - (seq_t)(2 * (n & 1));

    return result;
}
