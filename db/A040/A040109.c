/*
 * A040109  Continued fraction for sqrt(120).
 * https://oeis.org/A040109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040109: Continued fraction for sqrt(120)
   a(0) = 10
   a(n) = 20 if n is even and n >= 1
   a(n) = 1   if n is odd  (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)10;
    }
    return (n % 2 == 0) ? (seq_t)20 : (seq_t)1;
}
