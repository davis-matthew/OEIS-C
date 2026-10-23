/*
 * A140345  a(n) = a(n-1)^2 - a(n-2) - a(n-3) - a(n-4), a(1) = a(2) = a(3) =
 *          a(4) = 1.
 * https://oeis.org/A140345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)^2 – a(n-2) – a(n-3) – a(n-4)
   Initial values: a(1) = a(2) = a(3) = a(4) = 1
   The sequence is periodic with period 5: 1, 1, 1, 1, -2, ... */
seq_t a(idx_t n)
{
    /* return -2 for indices that are multiples of 5, otherwise 1 */
    return (n % 5 == 0) ? (seq_t)-2 : (seq_t)1;
}
