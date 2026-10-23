/*
 * A186809  Period 6 sequence [0, 1, 2, 0, -2, -1, ...].
 * https://oeis.org/A186809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = [0, 1, 2, 0, -2, -1] periodic with period 6
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 6 */
    idx_t r = n % 6;
    switch (r)
    {
    case 0: return (seq_t)0;
    case 1: return (seq_t)1;
    case 2: return (seq_t)2;
    case 3: return (seq_t)0;
    case 4: return (seq_t)(-2);
    case 5: return (seq_t)(-1);
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
