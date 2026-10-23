/*
 * A040928  Continued fraction for sqrt(959).
 * https://oeis.org/A040928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, mult, easy
 * Formula:   a(0) = 30; for n >= 1: if n % 4 == 0 -> 60 if n % 4 == 1 -> 1 if
 *            n % 4 == 2 -> 29 if n % 4 == 3 -> 1 (equivalently a(n) = a(n-4)
 *            for n >= 5)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)30;
    }
    idx_t r = n % 4;
    if (r == 0u) {
        return (seq_t)60;
    } else if (r == 1u || r == 3u) {
        return (seq_t)1;
    } else { /* r == 2 */
        return (seq_t)29;
    }
}
