/*
 * A040756  Continued fraction for sqrt(785).
 * https://oeis.org/A040756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040756
   Formula: a(0) = 28, a(n) = 56 for all n >= 1
*/
seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)28 : (seq_t)56;
}
