/*
 * A040425  Continued fraction for sqrt(447).
 * https://oeis.org/A040425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 21
   For n >= 1: a(n) = 7 if n is odd, otherwise 42
   (the continued fraction of √447 has the period [7, 42] after the initial 21) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)21;
    }
    /* n is >= 1 */
    return (n & 1) ? (seq_t)7 : (seq_t)42;
}
