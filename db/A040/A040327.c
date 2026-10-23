/*
 * A040327  Continued fraction for sqrt(346).
 * https://oeis.org/A040327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 18 if n==0, 36 if 5|n, 1 otherwise
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)18;
    return (n % 5 == 0) ? (seq_t)36 : (seq_t)1;
}
