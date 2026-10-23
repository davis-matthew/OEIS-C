/*
 * A026208  Duplicate of A026184.
 * https://oeis.org/A026208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = * if n mod 4 = 0, a = n/2 * if n mod 4 = 1, a = (3n-1)/2
 *            * if n mod 4 = 2, a = 3n/2 * if n mod 4 = 3, a = (3n+1)/2
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    switch (n & 3)
    {
    case 0:  /* n mod 4 == 0 */
        return nn >> 1;           /* n/2 */
    case 1:  /* n mod 4 == 1 */
        return (3 * nn - 1) / 2;
    case 2:  /* n mod 4 == 2 */
        return (3 * nn) / 2;
    case 3:  /* n mod 4 == 3 */
        return (3 * nn + 1) / 2;
    default:
        /* unreachable, but return 0 to silence compiler warnings */
        return 0;
    }
}
