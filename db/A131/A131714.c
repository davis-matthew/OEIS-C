/*
 * A131714  Period 6: repeat [1, -2, 2, -1, 2, -2].
 * https://oeis.org/A131714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1,-2,2,-1,2,-2][n mod 6]
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned r = (unsigned)(n % 6); /* remainder 0..5 */
    switch (r) {
    case 0: return 1;
    case 1: return -2;
    case 2: return 2;
    case 3: return -1;
    case 4: return 2;
    case 5: return -2;
    default: return 0; /* unreachable */
    }
}
