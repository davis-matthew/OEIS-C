/*
 * A022974  a(n) = 18-n.
 * https://oeis.org/A022974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 18 - n
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 18, a(1) = 17, a(n) = 2*a(n-1) - a(n-2) */

extern seq_t a(idx_t n)
{
    return (seq_t)18 - (seq_t)n;
}
