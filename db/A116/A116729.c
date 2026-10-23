/*
 * A116729  Duplicate of A048840.
 * https://oeis.org/A116729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^2 + 3*n - 6 for n >= 3; a(0)=1, a(1)=2, a(2)=5
 * Range:     i8 n <= 10, i16 n <= 29, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        /* handle the first three terms explicitly */
        switch ((idx_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)5;
        }
    }
    seq_t nn = (seq_t)n;
    return nn * nn + 3 * nn - 6;
}
