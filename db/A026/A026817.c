/*
 * A026817  Duplicate of A022269.
 * https://oeis.org/A026817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(11*n+1)/2 (OEIS A026817)
 * Range:     i8 n <= 4, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) { /* sequence is defined from n = 1, but guard anyway */
        return 0;
    }
    seq_t nn = (seq_t)n;
    return nn * (((seq_t)11 * nn) + (seq_t)1) / (seq_t)2;
}
