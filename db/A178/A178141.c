/*
 * A178141  Period 6: repeat [4, -1, 2, -4, 1, 2].
 * https://oeis.org/A178141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = period[n mod 6] with period {4, -1, 2, -4, 1, 2}
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence repeats every 6 terms: 4, -1, 2, -4, 1, 2. */
seq_t a(idx_t n)
{
    /* Periodic pattern */
    static const seq_t period[6] = {4, -1, 2, -4, 1, 2};
    return period[n % 6];
}
