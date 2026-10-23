/*
 * A040954  Continued fraction for sqrt(986).
 * https://oeis.org/A040954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuation fraction for sqrt(986):
   a(0) = 31,
   for n > 0: a(n) = 62 if n mod 3 == 0, otherwise a(n) = 2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)31;
    return ((n % 3) == 0 ? (seq_t)62 : (seq_t)2);
}
