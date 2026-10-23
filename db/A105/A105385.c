/*
 * A105385  Expansion of (1-x^2)/(1-x^5).
 * https://oeis.org/A105385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, 0, -1, 0, 0][n mod 5]
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 5)
    {
        case 0: return (seq_t)1;
        case 1: return 0;
        case 2: return -1;
        case 3: return 0;
        case 4: return 0;
        /* unreachable, but keep compiler happy */
        default: return 0;
    }
}
