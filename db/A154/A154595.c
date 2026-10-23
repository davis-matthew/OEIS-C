/*
 * A154595  Period 6: repeat [1, 3, 3, -1, -3, -3].
 * https://oeis.org/A154595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) + a(n-3) = 0 for n > 2
   Initial values: a(0) = 1, a(1) = 3, a(2) = 3
   This yields a periodic sequence with period 6:
   [1, 3, 3, -1, -3, -3] repeating. */

seq_t a(idx_t n)
{
    /* period values, stored as seq_t literals */
    static const seq_t period[6] = {1, 3, 3, -1, -3, -3};

    /* idx_t is unsigned, modulo 6 fits in a small integer */
    idx_t ix = n % 6;

    return period[ix];
}
